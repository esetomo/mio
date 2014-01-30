import sys
import os
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from pymio.exporter import bvh_exporter
from pymio.mmd.pmd import pmd_parser
from pymio.mmd.vmd import vmd_parser
from optparse import OptionParser

if __name__=="__main__":
    usage = 'usage: python script/vmd2bvh.py klass [options] PMD_FILE VMD_FILE [OUTPUT_FILE]'        
    option_parser = OptionParser(usage=usage)    
    option_parser.add_option("-l", "--lang",
                             dest="language",
                             help="the language to write joint names in ('english' or 'japanese')",
                             metavar="LANGUAGE",
                             default="english")
    option_parser.add_option("-s", "--scale",
                             dest="scaling_factor",
                             type="float",
                             metavar="SCALING_FACTOR",
                             help="how much to scale the skeleton",
                             default=1.0)    
    
    options, argv = option_parser.parse_args(sys.argv)
    
    if options.language not in ['english', 'japanese']:
        print "Language must be 'english' or 'japanese' (without the quote)"
        sys.exit(0)
    if options.scaling_factor <= 0:
        print "The scaling factor must be a positive real number."
        sys.exit(0)
    
    if len(argv) < 3:
        option_parser.print_help()
        sys.exit(0)
    
    print "Loading PMD file %s..." % argv[1]
    try:
        pmd_model = pmd_parser.parse(os.path.abspath(argv[1]))
    except Exception, e:
        print str(e)
        sys.exit(0)    
    
    print "Loading VMD file..."
    try:
        vmd_animation = vmd_parser.parse(os.path.abspath(argv[2]))
    except Exception, e:
        print str(e)
        sys.exit(0)
    
    first_frame, last_frame = vmd_animation.get_frame_range()    
    def progress_hook(frame):
        print "Frame #%d out of %d" % (frame, last_frame-first_frame+1)
        return True
    pose_animation = vmd_animation.get_ik_less_pose_animation(pmd_model, progress_hook)
    
    exported_content = bvh_exporter.export(pmd_model.get_rest_armature(), 
                                           pose_animation, 
                                           options.language, 
                                           30, 
                                           options.scaling_factor)

    if len(argv) < 4:
        model_name = os.path.split(os.path.abspath(argv[1]))[1].split('.')[0]        
        animation_path = os.path.abspath(argv[2])
        animation_name = os.path.split(animation_path)[1].split('.')[0]
        animation_dir = os.path.dirname(animation_path)
        bvh_file_name = os.path.abspath(os.path.join(animation_dir, model_name + "-" + animation_name + ".bvh"))        
    else:
        bvh_file_name = os.path.abspath(argv[3])
    
    print "Saving BVH data to %s" % bvh_file_name
    try:
        f = open(bvh_file_name, "wt")
        f.write(exported_content)
    except Exception, e:
        print str(e)
    finally:
        f.close()