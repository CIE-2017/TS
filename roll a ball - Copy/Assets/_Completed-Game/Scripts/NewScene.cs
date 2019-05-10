using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class NewScene : MonoBehaviour
{
    Text Error;

    public void LoadnextScene(string scenename)
    {
        bool isSuccess = true;
        PlayerStats.Id = Random.Range(0,100);
        if (isSuccess)
        {
            Application.LoadLevel(scenename);
        }
        else {
            Error.text = "Yourt ID or Password is Incorrect. Press try again.";
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        Error = GameObject.Find("Error").GetComponent<Text>();
        Error.text = "";
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
