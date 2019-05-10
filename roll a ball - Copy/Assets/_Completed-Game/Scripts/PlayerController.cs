using UnityEngine;

// Include the namespace required to use Unity UI
using UnityEngine.UI;

using System.Collections;

public class PlayerController : MonoBehaviour {
	
	// Create public variables for player speed, and for the Text UI game objects
	public float speed;
	public Text countText;
	public Text winText;

	// Create private references to the rigidbody component on the player, and the count of pick up objects picked up so far
	private Rigidbody rb;
	private int count;
    void Awake()
    {

    }
    // At the start of the game..
    void Start ()
	{
		// Assign the Rigidbody component to our private rb variable
		rb = GetComponent<Rigidbody>();
		// Set the count to zero 
		count = 0;

		// Run the SetCountText function to update the UI (see below)
		SetCountText ();

		// Set the text property of our Win Text UI to an empty string, making the 'You Win' (game over message) blank
		winText.text = "";
        GameObject client = GameObject.Find("SSL");
        SSL_client clientScript = client.GetComponent<SSL_client>();
        transform.position = clientScript.position;
        //Debug.Log(clientScript.position);
    }

	// Each physics step..
	void FixedUpdate ()
	{
        GameObject client = GameObject.Find("SSL");
        SSL_client clientScript = client.GetComponent<SSL_client>();
        Account json = clientScript.json_obj;
        string myId = clientScript.id;
        //transform.position = clientScript.position;
        if (json != null)
        {
            if (json.Id == myId)
            {
                // Set some local float variables equal to the value of our Horizontal and Vertical Inputs
                float moveHorizontal = float.Parse(json.Horizontal);
                float moveVertical = float.Parse(json.Vertical);

                // Create a Vector3 variable, and assign X and Z to feature our horizontal and vertical float variables above
                Vector3 movement = new Vector3(moveHorizontal, 0.0f, moveVertical);
                Debug.Log(movement);
                // Add a physical force to our Player rigidbody using our 'movement' Vector3 above, 
                // multiplying it by 'speed' - our public player speed that appears in the inspector

                //eiei.SetActive(true);
                rb.AddForce(movement * speed);
            }
        }
    }

	// When this game object intersects a collider with 'is trigger' checked, 
	// store a reference to that collider in a variable named 'other'..
	void OnTriggerEnter(Collider other) 
	{
		// ..and if the game object we intersect has the tag 'Pick Up' assigned to it..
		if (other.gameObject.CompareTag ("Pick Up"))
		{
			// Make the other game object (the pick up) inactive, to make it disappear
			other.gameObject.SetActive (false);
            other.gameObject.transform.position += new Vector3(2F,0,0);
            other.gameObject.SetActive(true);
            // Add one to the score variable 'count'
            count = count + 1;
           
            // Run the 'SetCountText()' function (see below)
            SetCountText ();
            transform.localScale += new Vector3(0.2F, 0.2F, 0.2F);
        }
	}

	// Create a standalone function that can update the 'countText' UI and check if the required amount to win has been achieved
	void SetCountText()
	{
        // Update the text field of our 'countText' variable
        //countText.text = "Count: " + count.ToString ();
        GameObject client = GameObject.Find("SSL");
        SSL_client clientScript = client.GetComponent<SSL_client>();
        string this_id = clientScript.room_id;
        countText.text = "Room ID:" +this_id;
		// Check if our 'count' is equal to or exceeded 12
		if (count >= 12) 
		{
			// Set the text value of our 'winText'
			winText.text = "You Win!";
		}
	}
}