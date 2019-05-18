import java.io.DataInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.Date;

class Task extends Thread {
	Socket sc = null;

	public Task(Socket sc) {
		this.sc = sc;
	}

	public void run() {
		byte[] inputByte = null;
		int length = 0;
		DataInputStream dis = null;
		FileOutputStream fos = null;
		try {
			try {
				dis = new DataInputStream(sc.getInputStream());
				SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd/HH-mm-ss");//设置日期格式
				String date = df.format(new Date());// new Date()为获取当前系统时间，也可使用当前时间戳
				System.out.println(date);
				fos = new FileOutputStream(new File("image/" + date + ".jpg"));
				inputByte = new byte[1024 * 4];
				System.out.println("开始接收数据...");
				while ((length = dis.read(inputByte, 0, inputByte.length)) > 0) {
					fos.write(inputByte, 0, length);
					fos.flush();
				}
				System.out.println("完成接收");
			} finally {
				if (fos != null)
					fos.close();
				if (dis != null)
					dis.close();
				if (sc != null)
					sc.close();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
}

public class Server {
	public static void main(String[] args) throws Exception {
		ServerSocket serverSocket = new ServerSocket(9090);
		while (true) {
			Socket accept = serverSocket.accept();
			new Task(accept).start();
		}
	}
}
