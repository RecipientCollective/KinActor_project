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
	/// The actor_id.
	/// </summary>
	public int actor_id = 1;
	
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
	
	// SKELY
	private bool showSkeleton = true;
	
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
	private bool _connected;
	
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
		
		Debug.Log("OniActor script Started. Attached to object: " + gameObject.name + " with child: " + skeleton.name + ". Tracking actor with id: " + actor_id);
		
		initSkeletonPosition();
		
		connect();		
	}
	
	/// <summary>
	/// Update this instance. Update is called once per frame
	/// </summary>
	void Update () 
	{
		if (Input.GetKeyDown(KeyCode.Z)) toggleSkeletonVisibility();		
		if (showSkeleton) updateSkeletonPoints();
	}
	
	/// <summary>
	/// Raises the destroy event.
	/// </summary>
	void OnDestroy()
	{
		disconnect();
	}
	
	/// <summary>
	/// Toggles the skeleton visibility.
	/// </summary>
	private void toggleSkeletonVisibility()
	{
		showSkeleton = !showSkeleton;
		// toggles the visibility of this gameobject childrens
		foreach (Transform child in skeleton.transform)
		{
			child.renderer.enabled = !child.renderer.enabled;
		}
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
		
		updateJointCoordinates(args, address);
	}
	
	/// <summary>
	/// Updates the joint coordinates.
	/// </summary>
	/// <param name='args'>
	/// Arguments.
	/// </param>
	/// <param name='address'>
	/// Address.
	/// </param>
	private void updateJointCoordinates(ArrayList args, string address)
	{
		float x,y,z;
		
		if (address == "/skeleton/" + actor_id + "/center")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];
			
			center = getJointPosition(new Vector3(x,y,z));
			
			Debug.Log("Tracking Skeleton Center: " + center);
		}
		else if (address == "/skeleton/" + actor_id + "/neck/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			neckStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			neckEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/hip/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			hipStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			hipEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/left_upper_torso/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			leftUpperTorsoStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			leftUpperTorsoEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/right_upper_torso/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			rightUpperTorsoStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			rightUpperTorsoEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/left_lower_torso/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			leftLowerTorsoStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			leftLowerTorsoEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/right_lower_torso/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			rightLowerTorsoStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			rightLowerTorsoEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/left_upper_leg/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			leftUpperLegStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			leftUpperLegEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/left_lower_leg/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			leftLowerLegStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			leftLowerLegEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/right_upper_leg/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			rightUpperLegStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			rightUpperLegEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/right_lower_leg/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			rightLowerLegStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			rightLowerLegEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/left_shoulder/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			leftShoulderStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			leftShoulderEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/right_shoulder/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			rightShoulderStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			rightShoulderEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/left_upper_arm/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			leftUpperArmStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			leftUpperArmEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/left_lower_arm/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			leftLowerArmStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			leftLowerArmEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/right_upper_arm/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			rightUpperArmStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			rightUpperArmEnd = getJointPosition(new Vector3(x,y,z));
		}
		else if (address == "/skeleton/" + actor_id + "/right_lower_arm/realWorld")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];			
			rightLowerArmStart = getJointPosition(new Vector3(x,y,z));
			x = (float) args[3];
			y = (float) args[4];
			z = (float) args[5];
			rightLowerArmEnd = getJointPosition(new Vector3(x,y,z));
		}
	}
		
	
	/// <summary>
	/// Inits the skeleton position.
	/// </summary>
	private void initSkeletonPosition()
	{
		Debug.Log("Init skeleton Vetor3 positions with position in scene.");
		
		// INIT SKELETON POSITION
		center = skeleton.transform.Find("SkeletonCenter").position;
		neckStart = skeleton.transform.Find("NeckStart").position;
		neckEnd = skeleton.transform.Find("NeckEnd").position;
		hipStart = skeleton.transform.Find("HipStart").position;
		hipEnd = skeleton.transform.Find("HipEnd").position;
		
		leftUpperTorsoStart = skeleton.transform.Find("LeftUpperTorsoStart").position;
		leftUpperTorsoEnd = skeleton.transform.Find("LeftUpperTorsoEnd").position;
		rightUpperTorsoStart = skeleton.transform.Find("RightUpperTorsoStart").position;
		rightUpperTorsoEnd = skeleton.transform.Find("RightUpperTorsoEnd").position;
		
		leftLowerTorsoStart = skeleton.transform.Find("LeftLowerTorsoStart").position;
		leftLowerTorsoEnd = skeleton.transform.Find("LeftLowerTorsoEnd").position;
		rightLowerTorsoStart = skeleton.transform.Find("RightLowerTorsoStart").position;
		rightLowerTorsoEnd = skeleton.transform.Find("RightLowerTorsoEnd").position;
		
		leftUpperLegStart = skeleton.transform.Find("LeftUpperLegStart").position;
		leftUpperLegEnd = skeleton.transform.Find("LeftUpperLegEnd").position;
		leftLowerLegStart = skeleton.transform.Find("LeftLowerLegStart").position;
		leftLowerLegEnd = skeleton.transform.Find("LeftLowerLegEnd").position;
		
		rightUpperLegStart = skeleton.transform.Find("RightUpperLegStart").position;
		rightUpperLegEnd = skeleton.transform.Find("RightUpperLegEnd").position;
		rightLowerLegStart = skeleton.transform.Find("RightLowerLegStart").position;
		rightLowerLegEnd = skeleton.transform.Find("RightLowerLegEnd").position;
		
		leftShoulderStart = skeleton.transform.Find("LeftShoulderStart").position;
		leftShoulderEnd = skeleton.transform.Find("LeftShoulderEnd").position;
		rightShoulderStart = skeleton.transform.Find("RightShoulderStart").position;
		rightShoulderEnd = skeleton.transform.Find("RightShoulderEnd").position;
		
		leftUpperArmStart = skeleton.transform.Find("LeftUpperArmStart").position;
		leftUpperArmEnd = skeleton.transform.Find("LeftUpperArmEnd").position;
		leftLowerArmStart = skeleton.transform.Find("LeftLowerArmStart").position;
		leftLowerArmEnd = skeleton.transform.Find("LeftLowerArmEnd").position;
		
		rightUpperArmStart = skeleton.transform.Find("RightUpperArmStart").position;
		rightUpperArmEnd = skeleton.transform.Find("RightUpperArmEnd").position;
		rightLowerArmStart = skeleton.transform.Find("RightLowerArmStart").position;
		rightLowerArmEnd = skeleton.transform.Find("RightLowerArmEnd").position;
		
		Debug.Log("Init skeleton Vetor3 positions complete.");
	}
	
	
	/// <summary>
	/// Updates the skeleton points.
	/// </summary>
	private void updateSkeletonPoints()
	{
		// UPDATE SKELETON POSITION SOME JOINTS ARE OVERLAPPING ...
		skeleton.transform.Find("SkeletonCenter").position = center;
		skeleton.transform.Find("NeckStart").position = neckStart;
		skeleton.transform.Find("NeckEnd").position = neckEnd;
		skeleton.transform.Find("HipStart").position = hipStart;
		skeleton.transform.Find("HipEnd").position = hipEnd;
		skeleton.transform.Find("LeftUpperTorsoStart").position = leftUpperTorsoStart;
		skeleton.transform.Find("LeftUpperTorsoEnd").position = leftUpperTorsoEnd;
		skeleton.transform.Find("RightUpperTorsoStart").position = rightUpperTorsoStart;
		skeleton.transform.Find("RightUpperTorsoEnd").position = rightUpperTorsoEnd;
		skeleton.transform.Find("LeftLowerTorsoStart").position = leftLowerTorsoStart;
		skeleton.transform.Find("LeftLowerTorsoEnd").position = leftLowerTorsoEnd;
		skeleton.transform.Find("RightLowerTorsoStart").position = rightLowerTorsoStart;
		skeleton.transform.Find("RightLowerTorsoEnd").position = rightLowerTorsoEnd;
		skeleton.transform.Find("LeftUpperLegStart").position = leftUpperLegStart;
		skeleton.transform.Find("LeftUpperLegEnd").position = leftUpperLegEnd;
		skeleton.transform.Find("LeftLowerLegStart").position = leftLowerLegStart;
		skeleton.transform.Find("LeftLowerLegEnd").position = leftLowerLegEnd;
		skeleton.transform.Find("RightUpperLegStart").position = rightUpperLegStart;
		skeleton.transform.Find("RightUpperLegEnd").position = rightUpperLegEnd;
		skeleton.transform.Find("RightLowerLegStart").position = rightLowerLegStart;
		skeleton.transform.Find("RightLowerLegEnd").position = rightLowerLegEnd;
		skeleton.transform.Find("LeftShoulderStart").position = leftShoulderStart;
		skeleton.transform.Find("LeftShoulderEnd").position = leftShoulderEnd;
		skeleton.transform.Find("RightShoulderStart").position = rightShoulderStart;
		skeleton.transform.Find("RightShoulderEnd").position = rightShoulderEnd;
		skeleton.transform.Find("LeftUpperArmStart").position = leftUpperArmStart;
		skeleton.transform.Find("LeftUpperArmEnd").position = leftUpperArmEnd;
		skeleton.transform.Find("LeftLowerArmStart").position = leftLowerArmStart;
		skeleton.transform.Find("LeftLowerArmEnd").position = leftLowerArmEnd;
		skeleton.transform.Find("RightUpperArmStart").position = rightUpperArmStart;
		skeleton.transform.Find("RightUpperArmEnd").position = rightUpperArmEnd;
		skeleton.transform.Find("RightLowerArmStart").position = rightLowerArmStart;
		skeleton.transform.Find("RightLowerArmEnd").position = rightLowerArmEnd;
	}
	
	/// <summary>
	/// Gets the joint position.
	/// </summary>
	/// <returns>
	/// The joint position.
	/// </returns>
	/// <param name='pos'>
	/// Position.
	/// </param>
	private Vector3 getJointPosition(Vector3 pos)
	{
		// IN MILLIMITERS: I DIVIDE BY 1000 Unity Scene 1 = 1m
		pos = pos / 1000.0f;
		// ADJUST Y WITH THE KINECT HEIGHT: if kinect is at 1.5m FLOOR ZERO IS -1.5m
		pos.y = pos.y + kinect_height;
		// REVERSE Z
		pos.z = -pos.z;
		return pos;
	}
	
	
	/// <summary>
	/// Raises the draw gizmos selected event.
	/// </summary>
	void OnDrawGizmosSelected() 
	{
        Gizmos.color = new Color(1, 0, 0, 0.1F);
        Gizmos.DrawCube(transform.position, new Vector3(1, 1, 1));
    }
}
