using UnityEngine;
using System.Collections;
using TUIO;
using System.Collections.Generic;

public class BBInputDelegate : MonoBehaviour {
	
	public Dictionary<long,BBTouchEvent> activeEvents = new Dictionary<long,BBTouchEvent>(100);
	protected ArrayList eventQueue = new ArrayList();
	protected Object eventQueueLock = new Object();
	
	protected BBInputController tuioInput;
	
	protected float cameraPixelWidth;
	protected float cameraPixelHeight;
	
	// Use this for initialization
	void Awake () {
		tuioInput = new BBInputController();
		tuioInput.eventDelegate = this;
		cameraPixelWidth = Camera.main.pixelWidth;
		cameraPixelHeight = Camera.main.pixelHeight;
		DontDestroyOnLoad(this);
		setup();
	}

	public virtual void setup()
	{
		// for the children classes
	}

	public BBInputController inputController()
	{
		return tuioInput;
	}

	// Ensure that the instance is destroyed when the game is stopped in the editor.
	void OnApplicationQuit() 
	{
		if (tuioInput != null) {
			tuioInput.eventDelegate = null;
			tuioInput.disconnect();
		}
	}

	private void updateEvent(BBTouchEvent anEvent, TuioCursor cursor)
	{
		anEvent.lastScreenPosition = anEvent.screenPosition;
		anEvent.tuioPosition = new Vector2(cursor.getX(),(1.0f - cursor.getY()));
		anEvent.screenPosition = new Vector3(cursor.getX() * cameraPixelWidth,(1.0f - cursor.getY()) * cameraPixelHeight,0.3f); 
		anEvent.lastTouchTime = anEvent.touchTime;
		anEvent.touchTime = Time.time;
	}

	// Cursor down is for new touch events. we take the TUIO cursor object and convert it
	// into a touch event, and add it to our active list of events
	public virtual void cursorDown(TuioCursor cursor) {
		// first, make a new BBTouchEvent, tag it with the unique touch id
		BBTouchEvent newEvent = new BBTouchEvent(cursor.getSessionID()); 
		// set the initial information		
		newEvent.screenPosition = new Vector3(cursor.getX() * cameraPixelWidth,(1.0f - cursor.getY()) * cameraPixelHeight,0.3f); 
		newEvent.eventState = BBTouchEventState.Began;
		// set all the rest of the info
		updateEvent(newEvent,cursor);
		
		// add it to our active event dictionary so we can retireve it based on it's unique ID
		// some times badness happens and we get an error adding one here for some reason
		// it should not ever be the case that the ID is already there.
		// if it is, then we need to behave
		if (activeEvents.ContainsKey(cursor.getSessionID())) {
			// then something is not right.. remove the old one and add a new one
			activeEvents.Remove(cursor.getSessionID());
		}
		activeEvents.Add( cursor.getSessionID(), newEvent );
		// queue it up for processing
		lock (eventQueueLock) eventQueue.Add(newEvent);
	}
	
	public virtual  void cursorMove(TuioCursor cursor) {
		// find the matching event object, set th state to 'moved'
		// and update it with the new position info
		if (!activeEvents.ContainsKey(cursor.getSessionID())) return;
		BBTouchEvent anEvent = activeEvents[cursor.getSessionID()];
		updateEvent(anEvent,cursor);
		anEvent.eventState = BBTouchEventState.Moved;
		lock (eventQueueLock) eventQueue.Add(anEvent);
	}
	
	public virtual  void cursorUp(TuioCursor cursor) {
		// find the matching event object, set the state to 'ended'
		// and remove it from our actives
		if (!activeEvents.ContainsKey(cursor.getSessionID())) return;
		BBTouchEvent anEvent = activeEvents[cursor.getSessionID()];
		anEvent.eventState = BBTouchEventState.Ended;	
		lock (eventQueueLock) eventQueue.Add(anEvent);
		activeEvents.Remove( cursor.getSessionID() );	
	}
	
	
	public virtual void finishFrame() {
		// this is called when the TUIO fseq message comes through, and it is
		// the end of this cycle.
		// normally you would process the event Q here
		lock (eventQueueLock) eventQueue.Clear();		
	}
}
