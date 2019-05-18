import socketserver
import datetime
import cv2
from imutils import perspective
from imutils import contours
import numpy as np
import imutils
import os
import serial
import time


def midpoint(pta, ptb):
    return (pta[0] + ptb[0]) * 0.5, (pta[1] + ptb[1]) * 0.5


def dot_detection(file):
    image = cv2.imread(file)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    cv2.imshow("gray", gray)
    gray = cv2.GaussianBlur(gray, (7, 7), 0)

    # perform edge detection, then perform a dilation + erosion to close gaps in between object edges
    edged = cv2.Canny(gray, 50, 100)
    edged = cv2.dilate(edged, None, iterations=1)
    edged = cv2.erode(edged, None, iterations=1)
    cv2.imshow("edged", edged)
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
    cv2.imshow("image", image)


def pedestrian_detection(net, file):
    global num
    num = 0
    image = cv2.imread(file)
    h, w = image.shape[:2]
    cv2.imshow("input", image)
    im_tensor = cv2.dnn.blobFromImage(image, size=(300, 300), swapRB=True, crop=False)
    net.setInput(im_tensor)
    cv2Out = net.forward()
    print(cv2Out.shape)
    for detect in cv2Out[0, 0, :, :]:
        score = detect[2]
        if score > 0.4:
            num += 1
            left = detect[3] * w
            top = detect[4] * h
            right = detect[5] * w
            bottom = detect[6] * h
            cv2.rectangle(image, (int(left), int(top)), (int(right), int(bottom)), (0, 255, 255), 4)
    cv2.imwrite("image/result.jpg", image)
    cv2.imshow("detection-out", image)


class MyServer(socketserver.BaseRequestHandler):
    def handle(self):
        s = self.request
        now = datetime.datetime.now()
        file_path = now.strftime("image/%Y-%m-%d/")
        if not os.path.exists(file_path):
            os.makedirs(file_path)
        file_path = now.strftime("image/%Y-%m-%d/%H-%M-%S.jpg")
        print(file_path)
        fp = open(file_path, 'wb')
        flag = True
        while flag:
            data = s.recv(1024)
            if len(data) == 0:
                flag = False
            fp.write(data)  # 写入图片数据
        fp.close()
        # dot_detection(file_path)
        pedestrian_detection(net, file_path)
        if num == 0:
            result = "安全，没有红蜘蛛！\n"
        else:
            result = "注意，有{}只红蜘蛛。\n".format(num)
        s.sendall(bytes(result, 'utf-8'))

        time.sleep(1)
        with open("image/result.jpg", 'rb') as f:
            pic_data = f.read()
        s.sendall(pic_data)
        time.sleep(1)
        s.close()

        serial.write(bytes("#{}*".format(num), 'utf-8'))
        cv2.waitKey(10000) & 0xff
        serial.write(bytes("#{}*".format(num), 'utf-8'))  # 怎么保证接收可靠？？？
        cv2.destroyAllWindows()


if __name__ == '__main__':
    num = 0
    serial = serial.Serial('COM1', 115200)
    inference_pb_path = r'D:\Project\Pycharm\pedestrian_demo\train\models\model\frozen_inference_graph.pb'
    graph_txt_path = r'D:\Project\Pycharm\pedestrian_demo\train\models\model\graph.pbtxt'
    net = cv2.dnn.readNetFromTensorflow(inference_pb_path, graph_txt_path)
    server = socketserver.ThreadingTCPServer(('', 9090), MyServer)
    server.serve_forever()
