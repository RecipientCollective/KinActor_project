using UnityEngine;
using System.Collections;

public class MainMenuScript : MonoBehaviour 
{
	public int panel_length = 400;
	public int panel_height = 400;
	public int panel_padding = 40;
	
	void Awake()
    {       
        //Make this script persistent(Not destroy when loading a new level)
        DontDestroyOnLoad(this);

        Time.timeScale = 1.0f; //In case some scenes does not UN-pause..
    }
	
	void OnGUI()
	{
		//Detect if we're in the main menu scene
        if (Application.loadedLevel == 0)
        {
            MainMenuGUI();
        }
        else
        {
            //Game scene
            InSceneGUI();
        }
	}
	
	void InSceneGUI()
	{
		//Top-right
        GUILayout.BeginArea(new Rect(0, 0, Screen.width, 50));
        GUILayout.BeginHorizontal();
        GUILayout.FlexibleSpace();
        if (GUILayout.Button("Back to menu"))
        {
            Destroy(gameObject); //Otherwise we'd have two of these..
            Application.LoadLevel(0);
        }
        GUILayout.EndHorizontal();
        GUILayout.EndArea();
	}	
	
	void MainMenuGUI()
	{
				
		// Make a background box
		GUI.Box(new Rect(10,10,panel_length,panel_height), "Loader Menu");
		
		// Make the first button. If it is pressed, Application.Loadlevel (1) will be executed
		if(GUI.Button(new Rect(panel_padding,panel_padding,panel_height - panel_padding,30), "Step 1: test TUIO PLUGIN")) 
		{
			Application.LoadLevel(1);
		}

		// Make the second button.
		if(GUI.Button(new Rect(panel_padding,panel_padding + 30 ,panel_height - panel_padding,30), "Step 2: test OSC CUSTOM SERVER")) 
		{
			Application.LoadLevel(2);
		}
	}
}
