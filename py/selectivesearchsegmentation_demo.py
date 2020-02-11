# -*- coding: utf-8 -*-

'''
A program demonstrating the use and capabilities of a particular image segmentation algorithm described
in Jasper R. R. Uijlings, Koen E. A. van de Sande, Theo Gevers, Arnold W. M. Smeulders:
    "Selective Search for Object Recognition"
International Journal of Computer Vision, Volume 104 (2), page 154-171, 2013
Usage:
    ./selectivesearchsegmentation_demo.py input_image (single|fast|quality)
Use "a" to display less rects, 'd' to display more rects, "q" to quit.

OpenCV selectivesearch python demo 位于 /path/to/opencv_contrib/modules/ximgproc/samples/selectivesearchsegmentation_demo.py
'''

import cv2 as cv
import sys

if __name__ == '__main__':
    img_path = '../imgs/beach.png'
    img = cv.imread(img_path, cv.IMREAD_COLOR)
    # img = cv.imread(sys.argv[1])

    cv.setUseOptimized(True)
    cv.setNumThreads(8)

    gs = cv.ximgproc.segmentation.createSelectiveSearchSegmentation()
    gs.setBaseImage(img)

    select_mode = 'f'
    if (select_mode == 's'):
        # if (sys.argv[2][0] == 's'):
        gs.switchToSingleStrategy()

    elif (select_mode == 'f'):
        # elif (sys.argv[2][0] == 'f'):
        gs.switchToSelectiveSearchFast()

    elif (select_mode == 'q'):
        # elif (sys.argv[2][0] == 'q'):
        gs.switchToSelectiveSearchQuality()
    else:
        print(__doc__)
        sys.exit(1)

    rects = gs.process()
    nb_rects = 10

    while True:
        wimg = img.copy()

        for i in range(len(rects)):
            if (i < nb_rects):
                x, y, w, h = rects[i]
                cv.rectangle(wimg, (x, y), (x + w, y + h), (0, 255, 0), 1, cv.LINE_AA)

        cv.imshow("Output", wimg)
        c = cv.waitKey()

        if (c == 100):
            nb_rects += 10

        elif (c == 97 and nb_rects > 10):
            nb_rects -= 10

        elif (c == 113):
            break

    cv.destroyAllWindows()
