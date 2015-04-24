package com.jeidee.glooxforandroid;

import java.util.ArrayList;

public interface IMsgClientEvent {
    void onConnect();
    void onDisconnect(int e);
    void onRoster(ArrayList<RosterItemData> rosters);
}
