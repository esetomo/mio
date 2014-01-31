using System;
using System.Collections.Generic;
using System.Linq;
using MioSharp.Mmd.Pmd;
using MioSharp.IK;
using MioSharp.Rigging;
using MioSharp.Curve;
using System.Windows.Media.Media3D;
using MioSharp.Mmd.Vpd;

namespace MioSharp.Mmd.Vmd
{
    public class VmdAnimation
    {
        private readonly VmdParser.VmdHeader header;
        private readonly IReadOnlyCollection<VmdParser.VmdBoneFrame> boneFrames;
        private readonly IReadOnlyCollection<VmdParser.VmdExpressionFrame> expressionFrames;

        internal VmdAnimation(VmdParser.VmdData raw)
        {
            header = raw.Header;
            boneFrames = raw.BoneFrames;
            expressionFrames = raw.ExpressionFrames;
        }

        public int GetFirstFrame()
        {
            int firstFrame = int.MaxValue;

            foreach (var boneFrame in boneFrames)
                if (boneFrame.FrameNumber < firstFrame)
                    firstFrame = boneFrame.FrameNumber;

            return firstFrame;
        }

        public int GetLastFrame()
        {
            int lastFrame = int.MinValue;

            foreach (var boneFrame in boneFrames)
                if (boneFrame.FrameNumber > lastFrame)
                    lastFrame = boneFrame.FrameNumber;

            return lastFrame;
        }

        public PoseAnimation GetIkLessPoseAnimation(PmdModel pmdModel, Func<double, bool> progressHook)
        {
            var curves = GetBoneCurves();
            var ikArmature = pmdModel.GetIkArmature();
            var ikPose = new IKPose();
            var pose = new Pose();

            var iterators = new Dictionary<string, IEnumerator<ControlPoint<JointChange>>>();
            foreach (var item in curves)
            {
                var boneName = item.Key;
                var curve = item.Value;
                iterators[boneName] = curve.GetControlPointIterator();
            }

            var frames = new Dictionary<string, ControlPoint<JointChange>>();
            foreach (var item in iterators)
            {
                var boneName = item.Key;
                var iterator = item.Value;
                if (iterator.MoveNext())
                    frames[boneName] = iterator.Current;
            }

            var ikJointCurves = new Dictionary<string, Polyline<JointChange>>();
            foreach (var ikJoint in ikArmature.GetIkJoints())
                ikJointCurves[ikJoint.Name] = new Polyline_JointChange();

            while (frames.Count > 0)
            {
                var earliestTime = 1e20f;
                foreach (var item in frames)
                {
                    var boneName = item.Key;
                    var frame = item.Value;
                    if (frame.Time < earliestTime)
                        earliestTime = frame.Time;
                }

                var vpd_pose = GetVpdPose(curves, pmdModel, earliestTime);
                pose = pmdModel.GetIkLessPose(vpd_pose);

                foreach (var item in ikJointCurves)
                {
                    var ikJointName = item.Key;
                    var curve = item.Value;
                    curve.SetControlPoint(earliestTime, pose.GetJointChange(ikJointName));
                }

                var boneNamesWithEarliestTime = new List<string>();
                foreach (var item in frames)
                {
                    var boneName = item.Key;
                    var frame = item.Value;
                    if (Math.Abs(frame.Time - earliestTime) < 0.0001)
                        boneNamesWithEarliestTime.Add(boneName);
                }

                foreach (var boneName in boneNamesWithEarliestTime)
                {
                    var iterator = iterators[boneName];
                    if (iterator.MoveNext())
                    {
                        frames[boneName] = iterator.Current;
                    }
                    else
                    {
                        frames.Remove(boneName);
                    }
                }

                if (progressHook != null)
                {
                    var continuing = progressHook(earliestTime);
                    if (!continuing)
                        return null;
                }
            }

            var poseAnimation = new PoseAnimation();

            curves = new Dictionary<string,Polyline<JointChange>>();
            foreach (var boneFrame in boneFrames)
            {
                if (!curves.ContainsKey(boneFrame.BoneName))
                    curves[boneFrame.BoneName] = new Polyline_JointChange();
                var position = new Vector3D(boneFrame.Position.X, boneFrame.Position.Y, -boneFrame.Position.Z);
                // var z_flip_matrix = matrix4x4.scale(1, 1, -1);
                var orientation = new Quaternion(boneFrame.Orientation.X,
                                         boneFrame.Orientation.Y,
                                         -boneFrame.Orientation.Z,
                                         -boneFrame.Orientation.W);
                curves[boneFrame.BoneName].SetControlPoint(boneFrame.FrameNumber,
                                                           new JointChange(position, orientation));
            }
            foreach (var item in curves)
            {
                var boneName = item.Key;
                var curve = item.Value;
                if (pmdModel.BonesByName.ContainsKey(boneName))
                {
                    var bone = pmdModel.GetBoneByName(boneName);
                    if (bone.BoneType != Bone.BONE_IK)
                        poseAnimation.SetJointCurve(boneName, curve);
                }
            }
            foreach (var item in ikJointCurves)
            {
                var boneName = item.Key;
                var ikJointCurve = item.Value;
                poseAnimation.SetJointCurve(boneName, ikJointCurve);
            }

            foreach (var bone in pmdModel.Bones)
            {
                if (bone.BoneType == Bone.BONE_ROTATION_INFLUENCED)
                {
                    var sourceBone = pmdModel.Bones.ElementAt(bone.IkBoneIndex);
                    var boneCurve = new Polyline_JointChange();
                    if (curves.ContainsKey(sourceBone.Name))
                    {
                        var sourceCurve = curves[sourceBone.Name];
                        foreach (var controlPoint in sourceCurve.GetControlPoints())
                        {
                            var time = controlPoint.Time;
                            var jc = controlPoint.Value;
                            boneCurve.SetControlPoint(time, new JointChange(new Vector3D(0, 0, 0), jc.Orientation));
                        }
                    }
                    poseAnimation.SetJointCurve(bone.Name, boneCurve);
                }
            }
            poseAnimation.UpdateRange();
            return poseAnimation;
        }

        private VpdPose GetVpdPose(Dictionary<string, Polyline<JointChange>> curves, PmdModel pmdModel, float time)
        {
            var vpdPose = new VpdPose();
            foreach (var item in curves)
            {
                var boneName = item.Key;
                var curve = item.Value;
                var jointChange = curve.Evaluate(time);
                vpdPose.SetJointChange(boneName, jointChange);
            }

            return vpdPose;
        }

        private Dictionary<string, Polyline<JointChange>> GetBoneCurves()
        {
            var curves = new Dictionary<string, Polyline<JointChange>>();
            foreach (var boneFrame in boneFrames)
            {
                if (!curves.ContainsKey(boneFrame.BoneName))
                    curves[boneFrame.BoneName] = new Polyline_JointChange();

                var position = boneFrame.Position;
                var orientation = boneFrame.Orientation;
                curves[boneFrame.BoneName].SetControlPoint(boneFrame.FrameNumber, new JointChange(position, orientation));
            }

            return curves;
        }
    }
}
