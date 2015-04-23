package com.jeidee.glooxforandroid;

import android.util.Log;

/**
 * Created by jeidee on 15. 4. 23..
 * Purpose      : 연결 실패 시 재연결 알고리즘 구현 및 관리
 */

public class BackoffManager {
    private int m_maxRetryCount             = 10;       // 최대 재시도 횟수
    private int m_initWaitBackoffSeconds    = 2;        // 현재 재시도 간격(단위: 초)
    private boolean m_noGiveupAndRepeat     = false;    // 최대 재시도 횟수를 초과할 경우 재시도 포기여부

    private int m_retryCount;
    private int m_waitBackoffSeconds;
    private int m_repeatCycle;          // 최대 재시도횟수를 초과한 횟수

    public void init(int maxRetryCount, int initWaitBackoffSeconds, boolean noGiveupAndRepeat) {
        m_maxRetryCount = maxRetryCount;
        m_initWaitBackoffSeconds = initWaitBackoffSeconds;
        m_noGiveupAndRepeat = noGiveupAndRepeat;

        m_retryCount = 0;
        m_waitBackoffSeconds = m_initWaitBackoffSeconds;
    }

    public boolean retry() throws InterruptedException {
        if (m_noGiveupAndRepeat == false && m_repeatCycle > 0) {
            return false;
        }

        m_retryCount++;
        m_waitBackoffSeconds *= 2;

        if (m_retryCount > m_maxRetryCount) {
            m_retryCount = 0;
            m_waitBackoffSeconds = m_initWaitBackoffSeconds;
            m_repeatCycle++;
        }

        // 대기
        Log.d("Retry", "연결 재시도중 ...");
        Thread.sleep(m_waitBackoffSeconds * 1000);

        return true;
    }
}
