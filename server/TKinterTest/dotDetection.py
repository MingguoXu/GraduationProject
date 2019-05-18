from tkinter import *
from tkinter.filedialog import askdirectory, askopenfilename
import os
import cv2

from imutils import perspective
from imutils import contours
import numpy as np
import imutils


def midpoint(pta, ptb):
    return (pta[0] + ptb[0]) * 0.5, (pta[1] + ptb[1]) * 0.5


def select_image_path():
    path_ = askopenfilename()
    image_path.set(path_)
    print(image_path.get())


def select_image_dir():
    path_ = askdirectory()
    image_dir.set(path_)
    print(image_dir.get())


def detection_one_pic():
    # load the image, convert it to grayscale, and blur it slightly
    image = cv2.imread(image_path.get())
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    cv2.imshow("gray", gray)
    cv2.imwrite("image/gray.jpg", gray)
    gray = cv2.GaussianBlur(gray, (7, 7), 0)
    cv2.imwrite("image/gaussianBlur.jpg", gray)
    # perform edge detection, then perform a dilation + erosion to close gaps in between object edges
    edged = cv2.Canny(gray, 50, 100)
    edged = cv2.dilate(edged, None, iterations=1)
    edged = cv2.erode(edged, None, iterations=1)
    cv2.imshow("edged", edged)
    cv2.imwrite("image/edged.jpg", edged)
    # find contours in the edge map
    cnts = cv2.findContours(edged.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    cnts = cnts[0] if imutils.is_cv2() else cnts[1]
    # sort the contours from left-to-right and initialize the 'pixels per metric' calibration variable
    (cnts, _) = contours.sort_contours(cnts)

    # loop over the contours individually
    for c in cnts:
        # if the contour is not sufficiently large, ignore it
        if cv2.contourArea(c) < 25:
            continue

        box = cv2.minAreaRect(c)
        box = cv2.cv.BoxPoints(box) if imutils.is_cv2() else cv2.boxPoints(box)
        box = np.array(box, dtype="int")

        # order the points in the contour such that they appear
        # in top-left, top-right, bottom-right, and bottom-left order, then draw the outline of the rotated bounding box
        box = perspective.order_points(box)
        cv2.drawContours(image, [box.astype("int")], -1, (0, 255, 0), 2)
        # loop over the original points and draw them
        for (x, y) in box:
            cv2.circle(image, (int(x), int(y)), 5, (0, 0, 255), -1)

        # unpack the ordered bounding box, then compute the midpoint
        # between the top-left and top-right coordinates, followed by
        # the midpoint between bottom-left and bottom-right coordinates
        (tl, tr, br, bl) = box
        (tltrX, tltrY) = midpoint(tl, tr)
        (blbrX, blbrY) = midpoint(bl, br)
        # compute the midpoint between the top-left and top-right points,
        # followed by the midpoint between the top-righ and bottom-right
        (tlblX, tlblY) = midpoint(tl, bl)
        (trbrX, trbrY) = midpoint(tr, br)
        # draw the midpoints on the image
        cv2.circle(image, (int(tltrX), int(tltrY)), 5, (255, 0, 0), -1)
        cv2.circle(image, (int(blbrX), int(blbrY)), 5, (255, 0, 0), -1)
        cv2.circle(image, (int(tlblX), int(tlblY)), 5, (255, 0, 0), -1)
        cv2.circle(image, (int(trbrX), int(trbrY)), 5, (255, 0, 0), -1)
        # draw lines between the midpoints
        cv2.line(image, (int(tltrX), int(tltrY)), (int(blbrX), int(blbrY)), (255, 0, 255), 2)
        cv2.line(image, (int(tlblX), int(tlblY)), (int(trbrX), int(trbrY)), (255, 0, 255), 2)
    # show the output image
    cv2.imshow("result", image)
    cv2.imwrite("image/result.jpg", image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


def detection_dir_pic():
    test_dir = image_dir.get()
    test = os.listdir(test_dir)
    test = [x for x in test if x[-4:] == '.jpg']
    cv2.namedWindow('Input')
    cv2.namedWindow('Detect')
    for f in test:
        file_path = os.path.join(test_dir, f)
        image = cv2.imread(file_path)
        cv2.imshow("Input", image)
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        gray = cv2.GaussianBlur(gray, (7, 7), 0)

        # perform edge detection, then perform a dilation + erosion to close gaps in between object edges
        edged = cv2.Canny(gray, 50, 100)
        edged = cv2.dilate(edged, None, iterations=1)
        edged = cv2.erode(edged, None, iterations=1)

        # find contours in the edge map
        cnts = cv2.findContours(edged.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        cnts = cnts[0] if imutils.is_cv2() else cnts[1]
        # sort the contours from left-to-right and initialize the 'pixels per metric' calibration variable
        (cnts, _) = contours.sort_contours(cnts)

        # loop over the contours individually
        for c in cnts:
            # if the contour is not sufficiently large, ignore it
            if cv2.contourArea(c) < 25:
                continue

            box = cv2.minAreaRect(c)
            box = cv2.cv.BoxPoints(box) if imutils.is_cv2() else cv2.boxPoints(box)
            box = np.array(box, dtype="int")

            # order the points in the contour such that they appear
            # in top-left, top-right, bottom-right, and bottom-left order, then draw the outline of the rotated bounding box
            box = perspective.order_points(box)
            cv2.drawContours(image, [box.astype("int")], -1, (0, 255, 0), 2)
            # loop over the original points and draw them
            for (x, y) in box:
                cv2.circle(image, (int(x), int(y)), 5, (0, 0, 255), -1)

            # unpack the ordered bounding box, then compute the midpoint
            # between the top-left and top-right coordinates, followed by
            # the midpoint between bottom-left and bottom-right coordinates
            (tl, tr, br, bl) = box
            (tltrX, tltrY) = midpoint(tl, tr)
            (blbrX, blbrY) = midpoint(bl, br)
            # compute the midpoint between the top-left and top-right points,
            # followed by the midpoint between the top-righ and bottom-right
            (tlblX, tlblY) = midpoint(tl, bl)
            (trbrX, trbrY) = midpoint(tr, br)
            # draw the midpoints on the image
            cv2.circle(image, (int(tltrX), int(tltrY)), 5, (255, 0, 0), -1)
            cv2.circle(image, (int(blbrX), int(blbrY)), 5, (255, 0, 0), -1)
            cv2.circle(image, (int(tlblX), int(tlblY)), 5, (255, 0, 0), -1)
            cv2.circle(image, (int(trbrX), int(trbrY)), 5, (255, 0, 0), -1)
            # draw lines between the midpoints
            cv2.line(image, (int(tltrX), int(tltrY)), (int(blbrX), int(blbrY)), (255, 0, 255), 2)
            cv2.line(image, (int(tlblX), int(tlblY)), (int(trbrX), int(trbrY)), (255, 0, 255), 2)
        # show the output image
        cv2.imshow("Detect", image)
        c = cv2.waitKey(0) & 0xff
        if c == 27:
            break
    cv2.destroyAllWindows()


if __name__ == '__main__':
    # 创建窗口
    root = Tk()
    # 标题
    root.title('红蜘蛛检测模型')
    # 窗口大小  出现位置
    root.geometry('620x400')
    root.geometry('+500+200')
    # 变量
    image_path = StringVar()
    image_dir = StringVar()
    # 控件设置
    Label(root, text="检测图片", font=('华文行楷', 20)).grid(row=2, column=0)
    Entry(root, textvariable=image_path, width=22, font=('微软雅黑', 20)).grid(row=2, column=1)
    Button(root, text="路径选择", font=('微软雅黑', 20), command=select_image_path).grid(row=2, column=2)
    button = Button(root, text='检测单张', font=('微软雅黑', 20), command=detection_one_pic).grid(row=3, column=1)
    Label(root, text="图片文件", font=('华文行楷', 20)).grid(row=4, column=0)
    Entry(root, textvariable=image_dir, width=22, font=('微软雅黑', 20)).grid(row=4, column=1)
    Button(root, text="路径选择", font=('微软雅黑', 20), command=select_image_dir).grid(row=4, column=2)
    button = Button(root, text='检测全部', font=('微软雅黑', 20), command=detection_dir_pic).grid(row=5, column=1)
    root.mainloop()
