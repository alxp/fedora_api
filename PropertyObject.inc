<?php

/**
 * Description of PropertyObject
 *
 * From example code at http://www.php.net/manual/en/language.oop5.properties.php#98267
 */
abstract class PropertyObject {
  private $_construct = FALSE;
  
  public function __get($name) {
    if (method_exists($this, ($method = 'get_'.$name))) {
      return $this->$method();
    }
    else return;
  }
  
  public function __isset($name) {
    if (method_exists($this, ($method = 'isset_'.$name))){
      return $this->$method();
    }
    else return;
  }
  
  public function __set($name, $value) {
    if (method_exists($this, ($method = 'set_'.$name)) && !$this->_construct) {
      $this->$method($value);
    }
    else {
      // We're creating the object, don't bother with setters.
      $this->$name = $value;
    }
  }
  
  public function __unset($name) {
    if (method_exists($this, ($method = 'unset_'.$name))){
      $this->$method();
    }
  }
}
