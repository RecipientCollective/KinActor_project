using UnityEngine;
using System;
using System.Threading;
using System.Collections;
using System.Collections.Generic;

using OSC.NET;

public class TestOscReceiver : MonoBehaviour 
{
	private OSCReceiver receiver;
	private Thread thread;
	public int port = 12345;
		
	private bool _connected;
	public bool connected
	{
		get { return _connected; }
	}
	
	private int packetCount = 0;
	private int packetErrorCount = 0;
	private int bundleCount = 0;
	private int messageCount = 0;
	
	// Use this for initialization
	void Start () 
	{
		Debug.Log("OSC RECEIVER: Starting OSC server on secondary Thread");
		connect();
		Debug.Log("OSC RECEIVER: Starting OSC server complete");
	}
	
	// Update is called once per frame
	void Update () 
	{
	
	}
	
	// called when the MonoBehaviour will be destroyed.
	void OnApplicationQuit()
	{
		disconnect();
	}
	
	// CONNECT TO OSC SERVER AND START NEW THREAD
	private void connect()
	{
				
		try 
		{
			receiver = new OSCReceiver(port);
			_connected = true;
			Debug.Log("OSC SERVER: connecting to port: " + port + ". Thread STARTED with listen() method");
			thread = new Thread(new ThreadStart(listen));
			thread.Start();			
		} 
		catch (Exception e) 
		{
			Debug.Log("OSC SERVER: failed to connect to port " + port);
			Debug.Log(e.Message);
		}
	}
		
	// CLOSE THREAD
	private void disconnect()
	{
		if (receiver!=null) receiver.Close();
		receiver = null;
		Debug.Log("OSC SERVER: Disconnected from OSC Server");	
		_connected = false;
	}
	
	// THREADED METHOD IS LISTENING TO OSC CALLS
	private void listen()
	{
		while (connected)
		{
			try 
			{
				OSCPacket packet = receiver.Receive();
				packetCount++;
				if (packet!=null) {
					if (packet.IsBundle()) {
						bundleCount++;
						ArrayList messages = packet.Values;
						for (int i=0; i<messages.Count; i++) {
							messageCount++;
							processMessage((OSCMessage)messages[i]);
						}
					} else processMessage((OSCMessage)packet);
				} else {
					packetErrorCount++;
					Debug.Log("OSC LISTEN: null packet");
				}
			}
			catch (Exception e)
			{
				Debug.Log("OSC LISTEN: failed to receive packets at port " + port);
				Debug.Log(e.Message);
			}
				
		}
	}
	
	private void processMessage(OSCMessage message) 
	{
		string address = message.Address;
		ArrayList args = message.Values;		
		Debug.Log ("OSC MESSAGE--->ADDRESS: " + address + " --> VALUES:");
		for (int i = 0; i < args.Count; i++) 
		{
			Debug.Log("VALUE: " + args[i].ToString() + " . TYPE: " + args[i].GetType());	
		}
	}
}
