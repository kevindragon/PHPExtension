--TEST--
Check for multi_http_get presence
--SKIPIF--
<?php if (!extension_loaded("multi_http_get")) print "skip"; ?>
--POST--
--GET--
--INI--
--FILE--
<?php 
echo "multi_http_get extension is available";
/*
	you can add regression tests for your extension here

  the output of your test code has to be equal to the
  text in the --EXPECT-- section below for the tests
  to pass, differences between the output and the
  expected text are interpreted as failure

	see php4/README.TESTING for further information on
  writing regression tests
*/
?>
--EXPECT--
multi_http_get extension is available
