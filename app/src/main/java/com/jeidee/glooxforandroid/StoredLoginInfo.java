package com.jeidee.glooxforandroid;

/**
 * Created by jeidee on 15. 4. 23..
 */
public class StoredLoginInfo {
    private String m_jid;
    private String m_pwd;
    private String m_host;
    private int m_port;

    public void init() {
        m_jid = "test1@bypass";
        m_pwd = "1234";
        m_host = "msg.ima0.com";
        m_port = 5223;
    }

    public void setNewInfo(String jid, String pwd, String host, int port) {
        m_jid = jid;
        m_pwd = pwd;
        m_host = host;
        m_port = port;
    }

    public String jid() {
        return m_jid;
    }

    public String pwd() {
        return m_pwd;
    }

    public String host() {
        return m_host;
    }

    public int port() {
        return m_port;
    }
}
