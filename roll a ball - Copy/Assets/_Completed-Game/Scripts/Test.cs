using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Test : MonoBehaviour
{
    public float speed;
    private Rigidbody rb;
    private GameObject eiei;
    // Start is called before the first frame update
    void Start()
    {
        eiei = GameObject.Find("/Opponent/Sphere");
        rb = eiei.GetComponent<Rigidbody>();
        eiei.SetActive(false);
    }
    void FixedUpdate()
    {
        GameObject client = GameObject.Find("Main Camera");
        SSL_client clientScript = client.GetComponent<SSL_client>();
        //string data = clientScript.answer;
        Account json = clientScript.json_obj;
        // Set some local float variables equal to the value of our Horizontal and Vertical Inputs
        float moveHorizontal = float.Parse(json.Horizontal);
        float moveVertical = float.Parse(json.Vertical);

        // Create a Vector3 variable, and assign X and Z to feature our horizontal and vertical float variables above
        Vector3 movement = new Vector3(moveHorizontal, 0.0f, moveVertical);

        // Add a physical force to our Player rigidbody using our 'movement' Vector3 above, 
        // multiplying it by 'speed' - our public player speed that appears in the inspector
        if (json.Id == "2")
        {
            eiei.SetActive(true);
            rb.AddForce(movement * speed);
        }
    }
}
