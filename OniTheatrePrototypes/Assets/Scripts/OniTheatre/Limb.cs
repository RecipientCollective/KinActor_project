using UnityEngine;
using System.Collections;

public class Limb : MonoBehaviour 
{
	
	/// <summary>
	/// The start position.
	/// </summary>
	public Transform startPosition;
	
	/// <summary>
	/// The end position.
	/// </summary>
	public Transform endPosition;
	
	/// <summary>
	/// Update this instance.
	/// </summary>
	void Update () 
	{
		LineRenderer lineRenderer = GetComponent<LineRenderer>();
		lineRenderer.SetPosition(0, startPosition.position);
		lineRenderer.SetPosition(1, endPosition.position);
	}
}
