#pragma strict

var prefab : GameObject;
var numberOfObjects = 20;

function Start () {
	 for (var i = 0; i < numberOfObjects; i++) {
	 	var pos = transform.position + Vector3(0,i,0);
	 	Instantiate(prefab, pos, Quaternion.identity);
	 }
}

function Update () {

}