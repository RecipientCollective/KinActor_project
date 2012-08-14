/// <summary>
/// Oni actor. OSC KINECT CONTROLLER
/// </summary>

using UnityEngine;
using System;
using System.Threading;
using System.Collections;
using System.Collections.Generic;
using OSC.NET;


public class OniActor : MonoBehaviour 
{
	/// <summary>
	/// The port.
	/// </summary>
	public int port = 12345;
	
	/// <summary>
	/// Hide show the skeleton structure
	/// </summary>
	bool showSkeleton = true;
	
	
	
	// PRIVATE STUFFS
	
	// OSC 
	private OSCReceiver receiver;
	private Thread thread;
	private int packetCount = 0;
	private int packetErrorCount = 0;
	private int bundleCount = 0;
	private int messageCount = 0;
	private bool _connected;
	
	// SKELETON JOINTS
	private Vector3 center;
	private Vector3 neckStart;
	private Vector3 neckEnd;
	private Vector3 hipStart;
	private Vector3 hipEnd;
	private Vector3 leftUpperTorsoStart;
	private Vector3 leftUpperTorsoEnd;
	private Vector3 rightUpperTorsoStart;
	private Vector3 rightUpperTorsoEnd;
	private Vector3 leftLowerTorsoStart;
	private Vector3 leftLowerTorsoEnd;
	private Vector3 rightLowerTorsoStart;
	private Vector3 rightLowerTorsoEnd;
	private Vector3 leftUpperLegStart;
	private Vector3 leftUpperLegEnd;
	private Vector3 leftLowerLegStart;
	private Vector3 leftLowerLegEnd;
	private Vector3 rightUpperLegStart;
	private Vector3 rightUpperLegEnd;
	private Vector3 rightLowerLegStart;
	private Vector3 rightLowerLegEnd;
	private Vector3 leftShoulderStart;
	private Vector3 leftShoulderEnd;
	private Vector3 rightShoulderStart;
	private Vector3 rightShoulderEnd;
	private Vector3 leftUpperArmStart;
	private Vector3 leftUpperArmEnd;
	private Vector3 leftLowerArmStart;
	private Vector3 leftLowerArmEnd;
	private Vector3 rightUpperArmStart;
	private Vector3 rightUpperArmEnd;
	private Vector3 rightLowerArmStart;
	private Vector3 rightLowerArmEnd;
	
	// GAME OBJECT CHILDS
	Transform skeleton;
	
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
	
	/// <summary>
	/// Start this instance. Use this for initialization
	/// </summary>//
	void Start () 
	{
		try 
		{
			skeleton = gameObject.transform.FindChild("Skeleton");	
		} 
		catch (Exception ex) 
		{
			Debug.LogError(gameObject.name + " should have a child named Skeleton." + ex.Message);
		}
		
		Debug.Log("OniActor script Started. Attached to object: " + gameObject.name + " with child: " + skeleton.name);
		connect();		
	}
	
	/// <summary>
	/// Update this instance. Update is called once per frame
	/// </summary>
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
		
	}
}
