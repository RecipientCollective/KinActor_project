using UnityEngine;
using System;
using System.Threading;
using System.Collections;
using System.Collections.Generic;

using OSC.NET;


public class TestOscSkeleton : MonoBehaviour 
{
	public Transform skeleton;
	public Transform colonnaKinect;
	
	public int port = 12345;
	public float kinect_height = 1.5f;
	
	private bool _connected;
	public bool connected
	{
		get { return _connected; }
	}
	
	private OSCReceiver receiver;
	private Thread thread;
	private int packetCount = 0;
	private int packetErrorCount = 0;
	private int bundleCount = 0;
	private int messageCount = 0;
	
	// SKELETON VECTORS
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
	
	
	// Use this for initialization
	void Start ()
	{
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
		
		Debug.Log("OSC RECEIVER: Starting OSC server on secondary Thread");
		connect();
		Debug.Log("OSC RECEIVER: Starting OSC server complete");
	}
	
	// Update is called once per frame
	void Update () 
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
		
		// COLONNA KINECT POSITION
		float y = kinect_height / 2.0f;
		Vector3 pos = new Vector3(colonnaKinect.position.x, y, colonnaKinect.position.z);
		colonnaKinect.position = pos;
		colonnaKinect.localScale = new Vector3(0.5f,kinect_height,0.5f);
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
		
		float x,y,z;
		
		// TEST WITH SKELETON 1 ONLY		
		if (address == "/skeleton/1/center")
		{
			x = (float) args[0];
			y = (float) args[1];
			z = (float) args[2];
			
			center = getJointPosition(new Vector3(x,y,z));
			
			Debug.Log("Tracking Skeleton Center: " + center);
		} 
		else if (address == "/skeleton/1/neck/realWorld")
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
		else if (address == "/skeleton/1/hip/realWorld")
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
		else if (address == "/skeleton/1/left_upper_torso/realWorld")
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
		else if (address == "/skeleton/1/right_upper_torso/realWorld")
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
		else if (address == "/skeleton/1/left_lower_torso/realWorld")
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
		else if (address == "/skeleton/1/right_lower_torso/realWorld")
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
		else if (address == "/skeleton/1/left_upper_leg/realWorld")
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
		else if (address == "/skeleton/1/left_lower_leg/realWorld")
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
		else if (address == "/skeleton/1/right_upper_leg/realWorld")
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
		else if (address == "/skeleton/1/right_lower_leg/realWorld")
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
		else if (address == "/skeleton/1/left_shoulder/realWorld")
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
		else if (address == "/skeleton/1/right_shoulder/realWorld")
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
		else if (address == "/skeleton/1/left_upper_arm/realWorld")
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
		else if (address == "/skeleton/1/left_lower_arm/realWorld")
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
		else if (address == "/skeleton/1/right_upper_arm/realWorld")
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
		else if (address == "/skeleton/1/right_lower_arm/realWorld")
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
}
