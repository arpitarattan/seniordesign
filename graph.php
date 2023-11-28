global $wpdb;
$results = $wpdb->get_results("SELECT id, test, value1 FROM SensorData", ARRAY_A);
// Transform the data into CanvasJS format
$dataPoints = array();
foreach ($results as $row) {
	if($row['test'] == 'r'){
		$dataPoint = array(
        'id' => $row['id'],
        'value1' => $row['value1']);
    	$dataPoints[] = $dataPoint;
	}
}

?>
<!DOCTYPE HTML>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Chart.js Example</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
    <canvas id="myChart" width="400" height="400"></canvas>

    <script>
        // Your JSON data
        var dataPoints = <?php echo json_encode($dataPoints, JSON_NUMERIC_CHECK); ?>;

        // Extract x and y values from JSON data
        var ids = dataPoints.map(item => item.id);
        var values = dataPoints.map(item => parseInt(item.value1, 10)); // Convert to integers

        // Get the canvas element
        var ctx = document.getElementById('myChart').getContext('2d');

        // Create the chart
        var myChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: ids, // x-axis data
                datasets: [{
                    label: 'Reaction Time',
                    data: values, // y-axis data
                    backgroundColor: 'rgba(75, 192, 192, 0.2)', // optional
                    borderColor: 'rgba(75, 192, 192, 1)', // optional
                    borderWidth: 1 // optional
                }]
            },
            options: {
                scales: {
                    x: {
                        type: 'linear',
       				 title: {
         			 display: true,
         			 text: 'Date'
        			},
                        position: 'bottom'
                    },
                    y: {
                        type: 'linear',
                        position: 'left',
						title: {
         				 display: true,
         			 		text: 'Reaction Time'
        			}
                    }
                }
            }
        });
    </script>
</body>
</html>
