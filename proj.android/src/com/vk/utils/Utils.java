package com.vk.utils;

import android.os.Environment;
import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.net.Uri;
import android.content.Intent;
import android.util.Log;
import java.io.*;

public class Utils {
    public static Activity activity = null;

    public static void setActivity(Activity activity) {
        Utils.activity = activity;
    }

    public static void composeLetter(String to, String subj) {
        if (activity == null) {
            return;
        }

        Intent intent = new Intent();
        intent.setClassName("com.google.android.gm", "com.google.android.gm.ComposeActivityGmail");
        String[] recipients = {to};
        intent.putExtra(Intent.EXTRA_EMAIL, recipients);
        intent.putExtra(Intent.EXTRA_SUBJECT, subj);
        intent.setType("text/html");
        activity.startActivity(Intent.createChooser(intent, "Send mail"));
    }

    public static void share(String title, String text, String picture) {
        Log.v("OLOLOXX", picture);
        Intent sendIntent = new Intent();
        sendIntent.setType("image/png");
        sendIntent.setAction(Intent.ACTION_SEND);
        File pictureFile = new File(picture);
        Uri screenshotUri = Uri.fromFile(pictureFile);
        sendIntent.putExtra(Intent.EXTRA_STREAM, screenshotUri);
        String marketUrl = "http://play.google.com/store/apps/details?id=" + activity.getPackageName();
        sendIntent.putExtra(Intent.EXTRA_TEXT, text + " " + marketUrl);
        activity.startActivity(Intent.createChooser(sendIntent, title));
    }

    public static String getExternalStoragePath() {
        String path = Environment.getExternalStorageDirectory() + "/";
        return path;
    }

    public static void copy(File src, File dst) throws IOException {
        InputStream in = new FileInputStream(src);
        OutputStream out = new FileOutputStream(dst);

        // Transfer bytes from in to out
        byte[] buf = new byte[1024];
        int len;
        while ((len = in.read(buf)) > 0) {
            out.write(buf, 0, len);
        }
        in.close();
        out.close();

         Log.v("OLOLOXX", "DONE");
    }

    public static void rate() {
        String packageName = activity.getPackageName();

        Uri uriDeep = Uri.parse("market://details?id=" + packageName);
        Uri uriDirect = Uri.parse("http://play.google.com/store/apps/details?id=" + packageName);

        try {
            Intent gotoMarketDeep = new Intent(Intent.ACTION_VIEW, uriDeep);
            gotoMarketDeep.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY |
                    Intent.FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET |
                    Intent.FLAG_ACTIVITY_MULTIPLE_TASK);
            activity.startActivity(gotoMarketDeep);
        } catch (ActivityNotFoundException e) {
            Intent gotoMarketDirect = new Intent(Intent.ACTION_VIEW, uriDirect);
            activity.startActivity(gotoMarketDirect);
        }
    }
}
