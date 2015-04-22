package com.jeidee.glooxforandroid;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.util.Log;

public class MainActivity extends ActionBarActivity implements IMsgClientEvent {

    private Button m_connectBtn;
    private Button m_recvBtn;
    private TextView m_textView;
    private MsgClient m_msgClient;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        m_connectBtn = (Button) findViewById(R.id.connectBtn);
        m_recvBtn = (Button) findViewById(R.id.recvBtn);

        m_textView = (TextView) findViewById(R.id.textVal);

        m_msgClient = MsgClient.Instance();

        m_msgClient.setMsgClientEvent(this);
        m_msgClient.init();

        m_connectBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            boolean ret = m_msgClient.connect("test1@bypass", "1234", "msg.iam0.com", 5223);
            if (ret)
                m_msgClient.run();

            m_textView.setText("call connectTest");
            }
        });

        m_recvBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            m_msgClient.recv();
            }
        });
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

    // implements IMsgClientEvent
    public void onConnect() {
        Log.d("MsgClientEvent", "onConnect");

    }

    public void onDisconnect(int e) {
        Log.d("MsgClientEvent", "onDisconnect");
    }
}
