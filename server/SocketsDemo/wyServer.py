import socketserver
from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw
import numpy as np
import time
import tflearn
from tflearn.layers.core import input_data, dropout, fully_connected
from tflearn.layers.conv import conv_2d, max_pool_2d
from tflearn.layers.normalization import local_response_normalization

def alex_net_model(model_file):
    """
    building 'AlexNet'
    :param model_file: 模型参数文件路径 'tmp/17flowers/my_model.tflearn'
    :return: 模型
    """
    # 定义输入
    network = input_data(shape=[None, 227, 227, 3])
    # 第一层卷积&池化
    network = conv_2d(network, 96, 11, strides=4, activation='relu')
    network = max_pool_2d(network, 3, strides=2)
    network = local_response_normalization(network)
    # 第二层卷积&池化
    network = conv_2d(network, 256, 5, activation='relu')
    network = max_pool_2d(network, 3, strides=2)
    network = local_response_normalization(network)
    # 第三层、四层卷积
    network = conv_2d(network, 384, 3, activation='relu')
    network = conv_2d(network, 384, 3, activation='relu')
    # 第五层卷积&池化
    network = conv_2d(network, 256, 3, activation='relu')
    network = max_pool_2d(network, 3, strides=2)
    network = local_response_normalization(network)
    # 第一层全连接层，以0.5的概率进行dropout
    network = fully_connected(network, 4096, activation='tanh')
    #network = dropout(network, 0.5)
    # 第二层全连接层
    network = fully_connected(network, 2048, activation='tanh')
    #network = dropout(network, 0.5)
    # 第三层全连接层，softmax输出分类结果
    network = fully_connected(network, 5, activation='softmax')

    model = tflearn.DNN(network)
    model.load(model_file)
    return model


class MyServer(socketserver.BaseRequestHandler):
    def handle(self):
        s = self.request
        file_path = "image/temp.jpg"
        print(file_path)
        fp = open(file_path, 'wb')
        flag = True
        while flag:
            data = s.recv(1024)
            if len(data) == 0:
                flag = False
            fp.write(data)  # 写入图片数据
        fp.close()

        img = Image.open(file_path)
        img = img.resize((227, 227), Image.ANTIALIAS)
        img = np.asarray(img, dtype="float32")
        img /= 255.
        result = model.predict(np.reshape(img, (1, 227, 227, 3)))
        class_id = result.argmax(-1)
        print(class_flower[class_id[0]])
        s.sendall(bytes(class_flower[class_id[0]], 'utf-8'))

        img = Image.open(file_path)
        draw = ImageDraw.Draw(img)
        draw.text((0, 0), class_flower[class_id[0]], (255, 255, 0), font=ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf', size=50))
        img.save(file_path)
        # img.show()

        time.sleep(1)
        with open(file_path, 'rb') as f:
            send_data = f.read()
        s.sendall(send_data)
        time.sleep(1)
        s.close()


if __name__ == '__main__':
    class_flower = ['发财树\n', '桂花\n',  '绿萝\n', '玫瑰\n', '栀子花\n', 'Cowslip\n', 'Buttercup\n', 'Windflower\n',
                    'Pansy\n', 'Lily Valley\n', 'Bluebell\n', 'Crocus\n', 'Iris\n', 'Tigerlily\n', 'Tulip\n', 'Fritillary\n', '向日葵\n']
    model = alex_net_model(r'/home/ubuntu/model/my_model.tflearn')
    server = socketserver.ThreadingTCPServer(('', 9090), MyServer)
    print("模型加载完成！开始服务...")
    server.serve_forever()
