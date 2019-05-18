import serial

if __name__ == '__main__':
    serial = serial.Serial('COM5', 115200)
    print(serial)
    if serial.isOpen():
        print("open success")
    else:
        print("open failed")

    try:
        while True:
            count = serial.inWaiting()
            if count > 0:
                data = serial.read(count)
                print("receive:", data)
                serial.write(data)
    except KeyboardInterrupt:
        if serial is not None:
            serial.close()
