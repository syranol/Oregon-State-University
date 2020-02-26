

$(document).ready(function(){
	$('#submitWeather').click(function(){
		
		var city = $("#city").val();

		if(city != '')
		{
			$.ajax({

			url: 'http://api.openweathermap.org/data/2.5/weather?q=' + city + "&APPID=3754fc19375147e116647cd881653e9d",
			type: "GET",
			dataType: "jsonp",
			success: function(data){
				var widget = show(data);

				$("#show").html(widget);

				$("#city").val('');
			}
		});

		}else{
			$("#error").html('Field cannot be empty');
		}
	});
});

$(document).ready(function(){
	$('#submitWeatherZip').click(function(){
		
		var zip = $("#zip").val();

		if(zip != '')
		{
			$.ajax({

			url: 'http://api.openweathermap.org/data/2.5/weather?zip=' + zip + "&APPID=3754fc19375147e116647cd881653e9d",
			type: "GET",
			dataType: "jsonp",
			success: function(data){
				var widget = showZip(data);

				$("#showZip").html(widget);

				$("#zip").val('');
			}
		});

		}else{
			$("#error").html('Field cannot be empty');
		}
	});
});

function show(data)
{

	return "<h3><strong>Weather</strong>: " + data.weather[0].main + "</h3>"+
	"<h3><strong>Description</strong>: " + data.weather[0].description + "</h3>"+
	"<h3><strong>Temperature</strong>: " + data.main.temp + "</h3>" +
	"<h3><strong>Plocatersure</strong>: " + data.main.plocatersure + "</h3>" +
	"<h3><strong>Humidity</strong>: " + data.main.humidity + "</h3>" +
	"<h3><strong>Temperature</strong>: " + data.main.temp_min + "</h3>" +
	"<h3><strong>Temperature</strong>: " + data.main.temp_max + "</h3>"+
	"<h3><strong>Speed</strong>: " + data.wind.speed + "</h3>" +
	"<h3><strong>Direction</strong>: " + data.wind.deg + "</h3>";
}

function showZip(data)
{

	return "<h3><strong>Weather</strong>: " + data.weather[0].main + "</h3>"+
	"<h3><strong>Description</strong>: " + data.weather[0].description + "</h3>"+
	"<h3><strong>Temperature</strong>: " + data.main.temp + "</h3>" +
	"<h3><strong>Plocatersure</strong>: " + data.main.plocatersure + "</h3>" +
	"<h3><strong>Humidity</strong>: " + data.main.humidity + "</h3>" +
	"<h3><strong>Temperature</strong>: " + data.main.temp_min + "</h3>" +
	"<h3><strong>Temperature</strong>: " + data.main.temp_max + "</h3>"+
	"<h3><strong>Speed</strong>: " + data.wind.speed + "</h3>" +
	"<h3><strong>Direction</strong>: " + data.wind.deg + "</h3>";
}


document.addEventListener('DOMContentLoaded', checkInput);

function checkInput()
{

	document.getElementById('submit').addEventListener('click', function(exercise){
		var req = new XMLHttpRequest();
		//as used fromt the teacher site
		var url = "http://httpbin.org/post";
		var info = {
			'fname': null,
			'lname': null,
		};
		info.fname = document.getElementById('ifname').value;
		info.lname = document.getElementById('ilname').value;
		req.open("POST", url, true);
		//content app load
		req.setRequestHeader('Content-Type', 'application/json');
		req.addEventListener('load', function() {
			if (req.status >= 200 && req.status < 400) 
			{
				var locater = JSON.parse(JSON.parse(req.responseText).data);
				console.log(locater);
				//Check for locater
				okToPrint(locater);
			}
			else 
			{
				var str = "Error in network request: " + locater.statusText;
				console.log(str);
				alert(str);
			}
		});
		//send the information
		req.send(JSON.stringify(info));
		exercise.preventDefault();
	});
}

function okToPrint(locater) {

	//return result
	document.getElementById('ofname').textContent = locater.fname;
	document.getElementById('olname').textContent = locater.lname;

}


//Credit. - This file contains information from an online tutorial 
//https://www.youtube.com/watch?v=RM56i0bY00o&list=PLAEoBV_GLyq4klW-2Pm75_5-r4oHhwqlm