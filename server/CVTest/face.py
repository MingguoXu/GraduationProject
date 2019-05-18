import cv2

# 加载opencv自带的人脸分类器
faceCascade = cv2.CascadeClassifier("haarcascade_frontalface_alt2.xml")
faceCascade.load('D:\Program Files\opencv\sources\data\haarcascades/haarcascade_frontalface_alt2.xml')

cap = cv2.VideoCapture(0)
flag = 0
timeF = 10
while True:
    flag += 1
    ret, frame = cap.read()
    img = frame.copy()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    rect = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.15,
        minNeighbors=3,
        minSize=(3, 3),
        flags=cv2.IMREAD_GRAYSCALE
    )
    for (x, y, w, h) in rect:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        # 识别到物体后进行裁剪保存
        # jiequ = img[x:(x+w), y:(y+h)]
        # cv2.imwrite('E://tools//python//eclipse//work//pythonTest//demo//0202//img//save//'+str(flag) + '.jpg',jiequ) #save as jpg

    # 读取到保存图片
    #     if(flag%timeF==0):
    #         cv2.imwrite('E://tools//python//eclipse//work//pythonTest//demo//0202//img//save//'+str(flag) + '.jpg',frame) #save as jpg

    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cap.release()
cv2.destroyAllWindows()
