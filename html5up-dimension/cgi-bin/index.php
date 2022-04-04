<!DOCTYPE HTML>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<title> GET / POST Php scripting </title>
	</head>
	<body>
		<h1>PHP Form Request</h1>
		<form action="cgi/post.php" method="get">
			<label for="name">Full Name: </label>
			<input type="text" id="name" name="name" autocomplete="off">
			<label for="email">Email: </label>
			<input type="text" id="email" name="email" autocomplete="off">
			<div class="button">
				<button type="submit">Submit</button>
				<button type="submit" formmethod="post">Submit with POST</button>
				<button type="reset">Reset</button>
			</div>
		</form>
	</body>
</html>
