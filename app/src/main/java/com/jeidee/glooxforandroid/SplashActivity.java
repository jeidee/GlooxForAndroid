package com.jeidee.glooxforandroid;

import android.content.Intent;
import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.os.Handler;
import android.os.AsyncTask;
import android.util.Log;

import java.util.ArrayList;

public class SplashActivity extends Activity {

    private LoginTask m_loginTask = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash);

        // 스플래시 화면 1초이상 보여 줌...
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        m_loginTask = new LoginTask();
        m_loginTask.execute((Void) null);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_splash, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public class LoginTask extends AsyncTask<Void, Void, Boolean> implements IMsgClientEvent {

        private Handler m_handler;
        private MsgClient m_msgClient;

        LoginTask() {
            m_handler = new Handler();

            m_msgClient = MsgClient.Instance();
            m_msgClient.init();
            m_msgClient.setHandler(m_handler, this);
        }

        @Override
        public void onConnect() {
            Log.d("Splash.MsgClientEvent", "onConnect");
        }

        @Override
        public void onDisconnect(int e) {
            Log.d("Splash.MsgClientEvent", String.format("onDisconnect(%d)", e));
        }

        @Override
        public void onRoster(ArrayList<RosterItemData> rosters) {
            Log.d("Splash.MsgClientEvent", "onRoster");
        }

        @Override
        protected Boolean doInBackground(Void... params) {
            if (m_msgClient.connected()) {
                return true;
            }

            if (!m_msgClient.connect()) {
                Log.e("Splash", "Connnect failed!");
                return false;
            }

            return true;
        }

        @Override
        protected void onPostExecute(final Boolean success) {
            m_loginTask = null;

            if (success) {
                Intent toMain = new Intent(SplashActivity.this, MainActivity.class);
                startActivity(toMain);
            } else {
                Intent toLogin = new Intent(SplashActivity.this, LoginActivity.class);
                startActivity(toLogin);
            }
            finish();
        }

        @Override
        protected void onCancelled() {
            m_loginTask = null;
        }
    }
}