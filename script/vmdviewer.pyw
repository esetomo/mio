import sys
import os
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from PyQt4 import QtCore, QtGui
from app.vmdviewer import main_window

if __name__ == '__main__':
    main_window.main()