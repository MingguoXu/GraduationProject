package com.xuzong.graduationdesign;

import android.Manifest;
import android.annotation.TargetApi;
import android.content.ContentUris;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.MediaScannerConnection;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.preference.PreferenceManager;
import android.provider.DocumentsContract;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.NavigationView;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v4.content.FileProvider;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBar;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.LogRecord;


public class MainActivity extends BaseActivity {

    private static final String TAG = "MainActivity";
    private DrawerLayout mDrawerLayout;
    public static final int TAKE_PHOTO = 1;
    public static final int CHOOSE_PHOTO = 2;
    private ImageView picture;
    private Uri imageUri;
    private String picFileFullName;
    private TextView textView;
    private String myResult;
    public static String addr = "192.168.1.12";
    public static int port = 9090;

    Handler myHandler = new Handler() {

        @Override
        public void handleMessage(Message msg) {
            switch(msg.what){
                case 1:
                    Bitmap bitmap = (Bitmap) msg.obj;
                    if(bitmap != null)
                        picture.setImageBitmap(bitmap);
                    else {
                        LogUtil.d(TAG, "结果图片接受失败");
                        Toast.makeText(MainActivity.this, "结果图片接受失败", Toast.LENGTH_SHORT).show();
                    }
                    break;
                case 2:
                    textView.setText(msg.obj + "");
                    break;
                default: break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        picture = (ImageView) findViewById(R.id.myImage);
        textView = (TextView) findViewById(R.id.myResult);
        //设置导航栏
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        //获取主界面（一个默认显示界面和隐藏菜单）
        mDrawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);
        //获取隐藏菜单，设置监听
        NavigationView navView = (NavigationView) findViewById(R.id.nav_view);
        navView.setNavigationItemSelectedListener(new NavigationView.OnNavigationItemSelectedListener() {
            @Override
            public boolean onNavigationItemSelected(@NonNull MenuItem item) {
                mDrawerLayout.closeDrawers();
                switch (item.getItemId()) {
                    case R.id.nav_call: {
                        Intent photoIntent = new Intent(Intent.ACTION_DIAL);
                        photoIntent.setData(Uri.parse("tel:15696576570"));
                        startActivity(photoIntent);
                        break;
                    }
                    case R.id.nav_friends: {
                        Toast.makeText(MainActivity.this, "You clicked Friends", Toast.LENGTH_SHORT).show();
                        break;
                    }
                    case R.id.nav_feedback: {
                        Intent feedbackIntent = new Intent(MainActivity.this, Feedback.class);
                        startActivity(feedbackIntent);
                        break;
                    }
                    case R.id.nav_help: {
                        Intent helpIntent = new Intent(MainActivity.this, Help.class);
                        startActivity(helpIntent);
                        break;
                    }
                    case R.id.nav_about: {
                        Intent aboutIntent = new Intent(MainActivity.this, About.class);
                        startActivity(aboutIntent);
                        break;
                    }
                    default:
                        break;
                }
                return true;
            }
        });
        //获取系统HomeAsUp键，并设置图标替换默认左箭头图标
        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setDisplayHomeAsUpEnabled(true);
            actionBar.setHomeAsUpIndicator(R.drawable.ic_menu);
        }
        //设置fab按钮监听
        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 创建File对象，用于存储拍照后的图片
                File outDir = new File(Environment.getExternalStorageDirectory() + "/红蜘蛛杀手/");
                if (!outDir.exists()) {
                    outDir.mkdirs();
                }
                String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
                File outputImage = new File(outDir, timeStamp + ".jpg");
                picFileFullName = outputImage.getAbsolutePath();
                try {
                    if (outputImage.exists()) {
                        outputImage.delete();
                    }
                    outputImage.createNewFile();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                if (Build.VERSION.SDK_INT >= 24) {
                    imageUri = FileProvider.getUriForFile(MainActivity.this,
                            "com.xuzong.graduationdesign.fileprovider", outputImage);
                } else {
                    imageUri = Uri.fromFile(outputImage);
                }
                // 启动相机程序
                Intent intent = new Intent("android.media.action.IMAGE_CAPTURE");
                intent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri);
                startActivityForResult(intent, TAKE_PHOTO);
            }
        });

        //登录
        Intent intent = new Intent(MainActivity.this, Login.class);
        startActivity(intent);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.toolbar, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home: {
                mDrawerLayout.openDrawer(GravityCompat.START);
                break;
            }
            case R.id.backup: {
                LogUtil.d(TAG, "You clicked Upload");
                Toast.makeText(MainActivity.this, "You clicked Upload", Toast.LENGTH_SHORT).show();
                new SendTask(picFileFullName).start();
                break;
            }
            case R.id.delete: {
                LogUtil.d(TAG, "You clicked Choose");
                Toast.makeText(MainActivity.this, "You clicked Choose", Toast.LENGTH_SHORT).show();

                break;
            }
            case R.id.settings: {
                LogUtil.d(TAG, "You clicked Settings");
                Toast.makeText(MainActivity.this, "You clicked Settings", Toast.LENGTH_SHORT).show();
                Intent settintIntent = new Intent(MainActivity.this, Setting.class);
                startActivity(settintIntent);
                break;
            }
            case R.id.exit: {
                //通过广播直接退出
                Intent intent = new Intent("com.xuzong.graduationdesign.FORCE_OFFLINE");
                sendBroadcast(intent);
                break;
            }
            default:
                break;
        }
        return true;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case TAKE_PHOTO:
                if (resultCode == RESULT_OK) {
                    try {
                        // 将拍摄的照片显示出来
                        Bitmap bitmap = BitmapFactory.decodeStream(getContentResolver().openInputStream(imageUri));
                        picture.setImageBitmap(bitmap);
                        //按比例压缩
                        getSmallBitmap();
                        //手动刷新，否则电脑中看不见
                        MediaScannerConnection.scanFile(this, new String[]{picFileFullName}, null, null);
                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    }
                }
                break;
            case CHOOSE_PHOTO:
                if (resultCode == RESULT_OK) {
                    // 判断手机系统版本号
                    if (Build.VERSION.SDK_INT >= 19) {
                        // 4.4及以上系统使用这个方法处理图片
                        handleImageOnKitKat(data);
                    } else {
                        // 4.4以下系统使用这个方法处理图片
                        handleImageBeforeKitKat(data);
                    }
                }
                break;
            default:
                break;
        }
    }

    @TargetApi(19)
    private void handleImageOnKitKat(Intent data) {
        String imagePath = null;
        Uri uri = data.getData();
        Log.d("TAG", "handleImageOnKitKat: uri is " + uri);
        if (DocumentsContract.isDocumentUri(this, uri)) {
            // 如果是document类型的Uri，则通过document id处理
            String docId = DocumentsContract.getDocumentId(uri);
            if ("com.android.providers.media.documents".equals(uri.getAuthority())) {
                String id = docId.split(":")[1]; // 解析出数字格式的id
                String selection = MediaStore.Images.Media._ID + "=" + id;
                imagePath = getImagePath(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, selection);
            } else if ("com.android.providers.downloads.documents".equals(uri.getAuthority())) {
                Uri contentUri = ContentUris.withAppendedId(Uri.parse("content://downloads/public_downloads"), Long.valueOf(docId));
                imagePath = getImagePath(contentUri, null);
            }
        } else if ("content".equalsIgnoreCase(uri.getScheme())) {
            // 如果是content类型的Uri，则使用普通方式处理
            imagePath = getImagePath(uri, null);
        } else if ("file".equalsIgnoreCase(uri.getScheme())) {
            // 如果是file类型的Uri，直接获取图片路径即可
            imagePath = uri.getPath();
        }
        displayImage(imagePath); // 根据图片路径显示图片
    }

    private void handleImageBeforeKitKat(Intent data) {
        Uri uri = data.getData();
        String imagePath = getImagePath(uri, null);
        displayImage(imagePath);
    }

    private String getImagePath(Uri uri, String selection) {
        String path = null;
        // 通过Uri和selection来获取真实的图片路径
        Cursor cursor = getContentResolver().query(uri, null, selection, null, null);
        if (cursor != null) {
            if (cursor.moveToFirst()) {
                path = cursor.getString(cursor.getColumnIndex(MediaStore.Images.Media.DATA));
            }
            cursor.close();
        }
        return path;
    }

    private void displayImage(String imagePath) {
        if (imagePath != null) {
            Bitmap bitmap = BitmapFactory.decodeFile(imagePath);
            picture.setImageBitmap(bitmap);
            picFileFullName = imagePath;
        } else {
            Toast.makeText(this, "failed to get image", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        switch (requestCode) {
            case 1:
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    openAlbum();
                } else {
                    Toast.makeText(this, "You denied the permission", Toast.LENGTH_SHORT).show();
                }
                break;
            default:
        }
    }

    private void openAlbum() {
        Intent intent = new Intent("android.intent.action.GET_CONTENT");
        intent.setType("image/*");
        startActivityForResult(intent, CHOOSE_PHOTO); // 打开相册
    }

    public Bitmap getSmallBitmap() {
        try {
            BitmapFactory.Options options = new BitmapFactory.Options();
            options.inSampleSize = 5;//缩放比例
            options.inJustDecodeBounds = false;//重新读入图片，注意此时把options.inJustDecodeBounds 设回false了
            Bitmap bitmap = BitmapFactory.decodeFile(picFileFullName, options);
            //压缩好比例大小后不进行质量压缩
            File f = new File(picFileFullName);
            if (f.exists()) {
                f.delete();
            }
            try {
                FileOutputStream out = new FileOutputStream(f);
                bitmap.compress(Bitmap.CompressFormat.JPEG, 100, out);//质量压缩方法，这里100表示不压缩，把压缩后的数据存放到bos中
                out.flush();
                out.close();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return bitmap;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    class SendTask extends Thread {
        String imagePath;

        public SendTask(String imagePath) {
            this.imagePath = imagePath;
        }

        public void run() {
            try {
                sendPic(imagePath);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private void sendPic(String imagePath) throws Exception {
        Socket socket = new Socket(addr, port);
        OutputStream out = socket.getOutputStream();
        DataOutputStream dos = new DataOutputStream(out);
        //输入流
        InputStream is = socket.getInputStream();
        BufferedReader br = new BufferedReader(new InputStreamReader(is));
        //文件
        FileInputStream fis = new FileInputStream(imagePath);
        try {
            byte[] buf = new byte[1024 * 4];
            int len = 0;
            while ((len = fis.read(buf, 0, buf.length)) != -1) {
                dos.write(buf, 0, len);
                dos.flush();
            }
            LogUtil.d(TAG, "是不是需要延时");
            socket.shutdownOutput();

            String reply = br.readLine();
            LogUtil.d(TAG, "接收服务器的信息：" + reply);
            myResult = "检测结果：" + reply;
            Message message = new Message();
            message.what = 2;
            message.obj = myResult;
            myHandler.sendMessage(message);

            DataInputStream dis = new DataInputStream(socket.getInputStream());
            String result_path = Environment.getExternalStorageDirectory() + "/红蜘蛛杀手/result.jpg";
            FileOutputStream fos = new FileOutputStream(result_path);
            while ((len = dis.read(buf, 0, buf.length)) > 0) {
                fos.write(buf, 0, len);
                fos.flush();
            }
            Message msg = new Message();
            msg.what = 1;
            msg.obj = BitmapFactory.decodeStream(new FileInputStream(result_path));
            myHandler.sendMessage(msg);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (fis != null)
                fis.close();
            if (out != null)
                out.close();
            if (dos != null)
                dos.close();
            if (socket != null)
                socket.close();
        }
    }
}
