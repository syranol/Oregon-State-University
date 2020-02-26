/*****************************************************************************
* Author: Hsiang Lo
* Date: 05/06/2018
* Description: In this file, script.js will display all the functionality of
1 - 3 as discussed in the homework assginment which consists of
1. Building a 4 x 4 table
2. Create directional buttons
3. And highlight marked cell when called
*****************************************************************************/
function addTable(){
    
    var buttonChart = document.body;
    
    var myTable = document.createElement("table");
    myTable.setAttribute('id', 'myTableID');
    myTable.setAttribute("border", "1");

    var tableHead = myTable.createTHead();

    var myTableTr = tableHead.insertRow(0);
    

    for(var i = 0 ; i < 4 ; i++){
        var headerCell = myTableTr.insertCell(i);
        headerCell.textContent = 'Header ' + (i+1);
    }
    
    var tableBody = myTable.createTBody();


    for(var i = 0 ; i < 3 ; i++){
        var currentRow = tableBody.insertRow(i);
        for(var j = 0 ; j < 4 ; j++){
            var currentCell = currentRow.insertCell(j);
            currentCell.textContent = (j+1).toString() + ", " + (i+1).toString();
        }
    }
    
    myTable.appendChild(tableHead);
    myTable.appendChild(tableBody);
    buttonChart.appendChild(myTable);
}

addTable();


function addButtons(){

    //Create Table
    var buttonChart = document.body;
    var myTable = document.createElement("table");

    var tableBody = myTable.createTBody();
    var currentRow = tableBody.insertRow(0);

    //Adds 5 buttons

    currentRow.insertCell(0);
    var currentCell = currentRow.insertCell(0);
    var thisButton = document.createElement('Button');

    var buttonText = document.createTextNode("Up");
    thisButton.appendChild(buttonText);
    thisButton.addEventListener("click", ifUp);

    currentCell.appendChild(thisButton);
 
    var currentCell = currentRow.insertCell(0);
    var thisButton = document.createElement('Button');

    var buttonText = document.createTextNode("Right");
    thisButton.appendChild(buttonText);
    thisButton.addEventListener("click", ifRight);

    currentCell.appendChild(thisButton);
   
    var currentCell = currentRow.insertCell(0);
    var thisButton = document.createElement('Button');

    var buttonText = document.createTextNode("Left");
    thisButton.appendChild(buttonText);
    thisButton.addEventListener("click", ifLeft);

    currentCell.appendChild(thisButton);
  
    var currentCell = currentRow.insertCell(0);
    var thisButton = document.createElement('Button');

    var buttonText = document.createTextNode("Down");
    thisButton.appendChild(buttonText);    
    thisButton.addEventListener("click", ifDown);

    currentCell.appendChild(thisButton);

    var currentCell = currentRow.insertCell(0);
    var thisButton = document.createElement('Button');

    var buttonText = document.createTextNode("Mark Cell");
    thisButton.appendChild(buttonText);
    thisButton.addEventListener("click", highlightBox);

    currentCell.appendChild(thisButton);

    myTable.appendChild(tableBody);
    buttonChart.appendChild(myTable);
}

//Call function to add buttons
addButtons();
var currentColumn = 0;
var newCell;
//Call (1,1) cell first
function selectCell()
{
    var thisTable = document.getElementById('myTableID');
    newCell = thisTable.children[1].children[0].firstElementChild;
    //2nd row, 1st column, first child
    newCell.style.border = '5px solid black';
    //newCell.setAttribute("border", "3"); //Current
}

//call select cell

selectCell();



//if up down right and left or highlight
function ifUp(Listened)
{
    var prevRow = newCell.parentElement.previousElementSibling;

    if((prevRow != null)&&(typeof(prevRow) == 'object') )
    { 
        newCell.style.border = '1px solid black';
        newCell = prevRow.children[currentColumn];
        newCell.style.border = '5px solid black';

    }
}

function ifDown(Listened)
{
    var nextRow = newCell.parentElement.nextElementSibling;
    
    if((nextRow != null) &&(typeof(nextRow) == 'object')){ 

        newCell.style.border = '1px solid black';
        newCell = nextRow.children[currentColumn];
        newCell.style.border = '5px solid black';

    }
}

function ifLeft(Listened)
{
    var prevCell = newCell.previousSibling;
    
    if((prevCell != null) && (typeof(prevCell) == 'object') )
    {
        currentColumn--;
        newCell.style.border = '1px solid black';
        newCell = prevCell;
        newCell.style.border = '5px solid black';
    } 
}

function ifRight(Listened)
{

    var nextCell = newCell.nextSibling;
    
    if((nextCell != null) && (typeof(nextCell) == 'object') )
    {
        currentColumn++;
        newCell.style.border = '1px solid black';
        newCell = nextCell;
        newCell.style.border = '5px solid black';
    }  

}

function highlightBox(Listened)
{
  newCell.style.backgroundColor = 'yellow';
}

