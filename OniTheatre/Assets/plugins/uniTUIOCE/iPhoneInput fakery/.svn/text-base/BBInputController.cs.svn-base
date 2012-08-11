// 080625 Jorgen Skogmo / shiftcontrol.dk
// Minimal implementation of a tuioclient

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using TUIO;

public class BBInputController : TuioListener {

	private TuioClient client;
	public BBInputDelegate eventDelegate;
	private bool cursorDidChange;

	public BBInputController() 
	{
		client = new TuioClient(3333);
		client.addTuioListener(this);
		client.connect();
		cursorDidChange = false;
	}

	public void disconnect() 
	{
		client.disconnect();
		client.removeTuioListener(this);
	}

	public bool isConnected()
	{
		return client.isConnected();
	}

	public int currentFrame()
	{
		return client.currentFrameNumber();		
	}
	
	public string getStatusString()
	{
		return client.getStatusString();		
	}
	

	// required implementations	
	public void addTuioObject(TuioObject o) {
		// cursorDidChange = true;
		// if (eventDelegate) eventDelegate.objectAdd(o);	
	}
	
	public void updateTuioObject(TuioObject o) {
		// cursorDidChange = true;
		// if (eventDelegate) eventDelegate.objectUpdate(o);	
	}
	
	public void removeTuioObject(TuioObject o) {
		// cursorDidChange = true;
		// if (eventDelegate) eventDelegate.objectRemove(o);
	}
	// 
	// for now we are only interested in cursor objects, ie touch events
	public void addTuioCursor(TuioCursor c) {
		cursorDidChange = true;
		if (eventDelegate) eventDelegate.cursorDown(c);	
	}
	public void updateTuioCursor(TuioCursor c) {
		cursorDidChange = true;
		if (eventDelegate) eventDelegate.cursorMove(c);	
	}
	public void removeTuioCursor(TuioCursor c) {
		cursorDidChange = true;
		if (eventDelegate) eventDelegate.cursorUp(c);	
	}
	
	// this is the end of a single frame
	// we only really need to call the frame end if something actually happened this frame
	public void refresh(TuioTime ftime) {
	
		if (!cursorDidChange) return;
		if (eventDelegate) eventDelegate.finishFrame();	
		cursorDidChange = false;
	}
}