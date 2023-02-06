<?php
//phpinfo();
$target_dir = "uploads/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));
$type = $_FILES["fileToUpload"]["type"];

// echo $_FILES["fileToUpload"]["size"];
// echo "<br>";
// Check if file already exists
if (file_exists($target_file)) {
  echo "Sorry, file already exists.<br>file<br>";
  echo $target_file;
  echo "<br>file type<br>";
  echo $imageFileType;
  echo "<br>";
  $uploadOk = 0;
}

// Check file size
if ($_FILES["fileToUpload"]["size"] > 10000) {
  echo "Sorry, your file is too large.<br>";
  $uploadOk = 0;
}

// Allow certain file formats
if($type == "") {
  echo "File format not supported!<br>";
  $uploadOk = 0;
}
// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
  echo "Sorry, your file was not uploaded.<br>";
// if everything is ok, try to upload file
} else {
  if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
    echo "Your note: ". htmlspecialchars( basename( $_FILES["fileToUpload"]["name"])). " has been uploaded.";
  } else {
    echo "Sorry not Sorry, there was an error uploading your file.<br>";
  }
}
?>