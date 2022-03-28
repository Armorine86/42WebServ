#!/usr/bin/php

Hi <?php echo htmlspecialchars($_POST['name']); ?>.
Your email is: <?php echo (int)$_POST['email']; ?>.
Your message is: <?php echo (int)$_POST['message']; ?>.
