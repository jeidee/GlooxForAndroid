package com.jeidee.glooxforandroid;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.util.Log;
import android.os.Handler;

import java.util.ArrayList;

public class MainActivity extends Activity implements IMsgClientEvent {

    private Handler m_handler;
    private MsgClient m_msgClient;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        m_handler = new Handler();

        m_msgClient = MsgClient.Instance();
        m_msgClient.init();
        m_msgClient.setHandler(m_handler, this);

        // 연결되었지만 RECV 쓰레드가 작동 중이지 않을 경우
        // 쓰레드 시작
        if (m_msgClient.connected() && !m_msgClient.running()) {
            m_msgClient.start();
        }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
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

    /*
     * IMsgClientEvent 인터페이스 구현
     */
    @Override
    public void onConnect() {
        Log.d("MsgClientEvent", "onConnect");
    }

    @Override
    public void onDisconnect(int e) {
        Log.d("MsgClientEvent", "onDisconnect");
    }

    @Override
    public void onRoster(ArrayList<RosterItemData> rosters) {
        Log.d("MsgClientEvent", "onRoster");

    }
}
