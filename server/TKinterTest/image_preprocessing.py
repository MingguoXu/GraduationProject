import os
import glob
from tkinter import *
from tkinter.filedialog import askdirectory, askopenfilename
import cv2
from random import randint
import numpy as np


def select_path():
    path_ = askdirectory()
    path.set(path_)
    print(path.get())


def detection_one_pic():
    extensions = ['jpg', 'jpeg']
    sub_dirs = [x[0] for x in os.walk(path.get())]
    print('----------分界线----------')
    label = 0
    is_root_dir = True
    for sub_dir in sub_dirs:
        if is_root_dir:
            is_root_dir = False
            root_dir = sub_dir
            continue
        file_list = []
        for extension in extensions:
            file_glob = os.path.join(sub_dir, '*.' + extension)
            file_list.extend(glob.glob(file_glob))
        if not file_list:
            continue
        print('processing:', sub_dir)
        cur_path = os.path.join(root_dir, '{:0>2d}'.format(label))
        if os.path.exists(cur_path) is not True:
            os.mkdir(cur_path)
        item = 0
        for file in file_list:
            print(file)
            image = cv2.imread(file)
            if image is None:
                print('图片读取错误')
                break
            # cv2.imshow('test', image)
            image = cv2.resize(image, (450, 600))
            cv2.imwrite(os.path.join(cur_path, '{:0>3d}.jpg'.format(item)), image)
            item += 1
        label += 1


def flip():
    extensions = ['jpg', 'jpeg']
    file_list = []
    for extension in extensions:
        file_glob = os.path.join(path.get(), '*.' + extension)
        file_list.extend(glob.glob(file_glob))
    if file_list is None:
        print('此文件夹下没有图片')
        return
    cur_path = os.path.join(path.get(), 'result')
    if os.path.exists(cur_path) is not True:
        os.mkdir(cur_path)
    item = 0
    for file in file_list:
        image = cv2.flip(cv2.imread(file), 1)
        cv2.imwrite(os.path.join(cur_path, '{:0>3d}_1.jpg'.format(item)), image)
        item += 1
        if item == 70:
            break


def rot90():
    extensions = ['jpg', 'jpeg']
    file_list = []
    for extension in extensions:
        file_glob = os.path.join(path.get(), '*.' + extension)
        file_list.extend(glob.glob(file_glob))
    if file_list is None:
        print('此文件夹下没有图片')
        return
    item = 0
    for file in file_list:
        image = cv2.imread(file)
        num = randint(0, 3)
        if num == 0:
            cv2.imwrite(file, image)
        elif num == 1:
            cv2.imwrite(file, np.rot90(image))
        else:
            cv2.imwrite(file, np.rot90(image, -1))


if __name__ == '__main__':
    root = Tk()
    path = StringVar()

    Label(root, text="目标路径:").grid(row=0, column=0)
    Entry(root, textvariable=path).grid(row=0, column=1)
    Button(root, text="路径选择", command=select_path).grid(row=0, column=2)
    Button(root, text='压缩图像', font=('微软雅黑', 20), command=detection_one_pic).grid(row=1, column=1)
    Button(root, text='镜像翻转', font=('微软雅黑', 20), command=flip).grid(row=2, column=1)
    Button(root, text='长宽旋转', font=('微软雅黑', 20), command=rot90).grid(row=3, column=1)

    root.mainloop()
