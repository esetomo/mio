from cmio import Polyline_float
from cmio import Polyline_Vector3
from cmio import Polyline_Quaternion
from cmio import Polyline_JointChange

if __name__=="__main__":
    curve = Polyline_float()
    curve.set_control_point(0, 0.0)
    curve.set_control_point(1, 1.0)
    print curve.evaluate(0.5)