from tkinter import *
from tkinter.filedialog import askdirectory, askopenfilename
import cv2
import os


def select_model_path():
    path_ = askopenfilename()
    inference_pb_path.set(path_)
    print(inference_pb_path.get())


def select_txt_path():
    path_ = askopenfilename()
    graph_txt_path.set(path_)
    print(graph_txt_path.get())


def select_image_path():
    path_ = askopenfilename()
    image_path.set(path_)
    print(image_path.get())


def select_image_dir():
    path_ = askdirectory()
    image_dir.set(path_)
    print(image_dir.get())


def detection_one_pic():
    net = cv2.dnn.readNetFromTensorflow(inference_pb_path.get(), graph_txt_path.get())
    image = cv2.imread(image_path.get())
    h, w = image.shape[:2]
    cv2.imshow("input", image)
    im_tensor = cv2.dnn.blobFromImage(image, size=(300, 300), swapRB=True, crop=False)
    net.setInput(im_tensor)
    cv2Out = net.forward()
    print(cv2Out.shape)
    for detect in cv2Out[0, 0, :, :]:
        score = detect[2]
        if score > 0.4:
            left = detect[3] * w
            top = detect[4] * h
            right = detect[5] * w
            bottom = detect[6] * h
            cv2.rectangle(image, (int(left), int(top)), (int(right), int(bottom)), (0, 255, 255), 4)
    cv2.imshow("detection-out", image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


def detection_dir_pic():
    net = cv2.dnn.readNetFromTensorflow(inference_pb_path.get(), graph_txt_path.get())
    test_dir = image_dir.get()
    test = os.listdir(test_dir)
    cv2.namedWindow('Input')
    cv2.namedWindow('Detect')
    for f in test:
        file_path = os.path.join(test_dir, f)
        image = cv2.imread(file_path)
        h, w = image.shape[:2]
        cv2.imshow("Input", image)
        im_tensor = cv2.dnn.blobFromImage(image, size=(300, 300), swapRB=True, crop=False)
        net.setInput(im_tensor)
        cv2Out = net.forward()
        print(cv2Out.shape)
        for detect in cv2Out[0, 0, :, :]:
            score = detect[2]
            if score > 0.4:
                left = detect[3] * w
                top = detect[4] * h
                right = detect[5] * w
                bottom = detect[6] * h
                cv2.rectangle(image, (int(left), int(top)), (int(right), int(bottom)), (0, 255, 255), 4)
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
    inference_pb_path = StringVar()
#    inference_pb_path.set(r'D:\Project\Pycharm\pedestrian_demo\train\models\model\frozen_inference_graph.pb')
    graph_txt_path = StringVar()
#    graph_txt_path.set(r'D:\Project\Pycharm\pedestrian_demo\train\models\model\graph.pbtxt')
    image_path = StringVar()
    image_dir = StringVar()
    # 控件设置
    Label(root, text="模型文件", font=('华文行楷', 20)).grid(row=0, column=0)
    Entry(root, textvariable=inference_pb_path, width=22, font=('微软雅黑', 20)).grid(row=0, column=1)
    Button(root, text="路径选择", font=('微软雅黑', 20), command=select_model_path).grid(row=0, column=2)
    Label(root, text="描述文件", font=('华文行楷', 20)).grid(row=1, column=0)
    Entry(root, textvariable=graph_txt_path, width=22, font=('微软雅黑', 20)).grid(row=1, column=1)
    Button(root, text="路径选择", font=('微软雅黑', 20), command=select_txt_path).grid(row=1, column=2)
    Label(root, text="检测图片", font=('华文行楷', 20)).grid(row=2, column=0)
    Entry(root, textvariable=image_path, width=22, font=('微软雅黑', 20)).grid(row=2, column=1)
    Button(root, text="路径选择", font=('微软雅黑', 20), command=select_image_path).grid(row=2, column=2)
    button = Button(root, text='检测单张', font=('微软雅黑', 20), command=detection_one_pic).grid(row=3, column=1)
    Label(root, text="图片文件", font=('华文行楷', 20)).grid(row=4, column=0)
    Entry(root, textvariable=image_dir, width=22, font=('微软雅黑', 20)).grid(row=4, column=1)
    Button(root, text="路径选择", font=('微软雅黑', 20), command=select_image_dir).grid(row=4, column=2)
    button = Button(root, text='检测全部', font=('微软雅黑', 20), command=detection_dir_pic).grid(row=5, column=1)
    root.mainloop()
