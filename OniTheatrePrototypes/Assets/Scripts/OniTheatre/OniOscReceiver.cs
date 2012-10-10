/// <summary>
/// Oni osc receiver.
/// </summary>
/// 

using UnityEngine;
using System;
using System.Threading;
using System.Collections;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using OSC.NET;


public class OniOscReceiver : MonoBehaviour {
	
	/// <summary>
	/// The port.
	/// </summary>
	public int port = 12345;
	
	public GameObject oniActorOne;
	public GameObject oniActorTwo;
	private OniActor oniActorOneScript;
	private OniActor oniActorTwoScript;
	
	/// <summary>
	/// The kinect_height.
	/// </summary>
	public float kinect_height = 1.5f; // in meters
	
	// PRIVATE =========================================
	
	// OSC 
	private OSCReceiver receiver;
	private Thread thread;
	private int packetCount = 0;
	private int packetErrorCount = 0;
	private int bundleCount = 0;
	private int messageCount = 0;
	
	// END PRIVATE =========================================
	
	/// <summary>
	/// Gets a value indicating whether this <see cref="OniActor"/> is connected to OSC server.
	/// </summary>
	/// <value>
	/// <c>true</c> if connected; otherwise, <c>false</c>.
	/// </value>
	public bool connected
	{
		get { return _connected; }
	}
	private bool _connected;
	
	
	
	
	/// <summary>
	/// Start this instance. Use this for initialization
	/// </summary>//
	void Start () 
	{
		oniActorOneScript = oniActorOne.GetComponent<OniActor>();
		oniActorTwoScript = oniActorTwo.GetComponent<OniActor>();
		connect();
	}
	
	
	/// <summary>
	/// Update this instance.
	/// </summary>
	// Update is called once per frame
	void Update () 
	{
	
	}
	
	/// <summary>
	/// Raises the destroy event.
	/// </summary>
	void OnDestroy()
	{
		disconnect();
	}
	
	/// <summary>
	/// Connect this instance.
	/// </summary>
	private void connect()
	{
		Debug.Log("ONIACTOR: Starting OSC server on secondary Thread");
		
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
			Debug.LogError("OSC SERVER: failed to connect to port " + port);
			Debug.LogError(e.Message);
		}
		
		Debug.Log("ONIACTOR: OSC server connection complete");
	}
	
	/// <summary>
	/// Disconnect this instance.
	/// </summary>/
	private void disconnect()
	{
		if (receiver!=null) receiver.Close();
		receiver = null;
		Debug.Log("OSC SERVER: Disconnected from OSC Server");	
		_connected = false;
	}
	
	/// <summary>
	/// Listen OSC messages;
	/// </summary>
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
	
	/// <summary>
	/// Processes the OSC message.
	/// </summary>
	/// <param name='message'>
	/// The Message.
	/// </param>
	private void processMessage(OSCMessage message) 
	{
		string address = message.Address;
		ArrayList args = message.Values;
		//Debug.Log("ADDRESS: " + address);
		// THIS IS HACK FIXME
		string actor_str = Regex.Replace(address,@"[/a-zA-Z_]*",String.Empty);
		int actor_id = Convert.ToInt32(actor_str);
		//Debug.Log("SKELETON ACTOR: " + actor_id);
		
		switch (actor_id) 
		{
		case 1: 
			oniActorOneScript.updateJointCoordinates(args, address); 
			break;
		case 2:
			oniActorTwoScript.updateJointCoordinates(args, address);
			break;
		default:
		break;
		}
	}
}