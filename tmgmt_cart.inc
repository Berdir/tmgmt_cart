<?php

class TMGMTCart {

  /**
   * Singleton instances of cart.
   */
  private static $instances;

  /**
   * Array key to store the contents of
   * $this->cart into the $_SESSION variable
   *
   * @var string
   */
  private $session_key;

  /**
   * An array to hold and manipulate
   * the contents of the job item cart
   *
   * @var array
   */
  private $cart;

  /**
   * Set up a new cart based on an existing $_SESSION key
   * or create a new on as an empty array with no items
   */
  public function __construct($session_key) {
    $this->session_key = $session_key;
    $this->cart = array_key_exists($this->session_key, $_SESSION) ? $_SESSION[$this->session_key] : array();
  }

  /**
   * Returns the singleton cart.
   */
  public static function getInstance($session_key = 'tmgmt_cart') {
    if (!isset(self::$instances[$session_key])) {
      self::$instances[$session_key] = new self($session_key);
    }
    return self::$instances[$session_key];
  }

  /**
   * Add job items to the cart
   *
   * @param array $items
   */
  public function addItems($type, array $items) {

    $source_map = $this->getSourceMap();

    foreach ($items as $key => $value) {
      $this->cart[$type . ':' . $value] = array(
        'source' => $source_map[$type],
        'type' => $type,
        'value' => $value,
        'label' => '(' . $source_map[$type] . ') ' . $type . ':' . $value,
      );
    }

    // save into the session
    $this->saveCart();

    drupal_set_message(t('The items have been added to your cart.'));
  }

  /**
   * Remove job items from the cart
   *
   * @param array $items
   */
  public function removeItems(array $items) {
    foreach ($items as $key => $value) {
      if (array_key_exists($key, $this->cart)) {
        drupal_set_message(t('Removed @item_name from the cart', array('@item_name' => $this->cart[$key]['label'])));
        unset($this->cart[$key]);
      }
    }
    $this->saveCart();
  }

  /**
   * View a list of job items in the cart in
   * a key => label format so they can be used
   * as $options in tmgmt_cart_form()
   *
   * @return array $items
   */
  public function getItems() {

    $items = array();

    foreach ($this->cart as $key => $item) {
      $items[$key] = $item['label'];
    }

    return $items;
  }

  /**
   * Create a job based on the current
   * items in the cart and empty the cart
   *
   */
  public function checkout() {

    global $user;

    $source_map = $this->getSourceMap();
    $job = tmgmt_job_create('en', NULL, $user->uid);

    foreach ($this->cart as $item) {
      $job->addItem($source_map[$item['type']], $item['type'], $item['value']);
    }

    // add a default translator
    $translators = tmgmt_translator_load_available($job);

    if (count($translators) !== 0) {
      $job->translator = current(array_keys($translators));
    }

    $job->save();

    $this->emptyCart();

    drupal_set_message(t('One job needs to be checked out.'));

  }

  /**
   * Remove all contents from the cart
   */
  public function emptyCart() {
    $this->cart = array();
    $this->saveCart();
    drupal_set_message(t('The cart has been emptied of its items.'));
  }

  /**
   * Save the current state of the cart
   * into the $_SESSION variable
   */
  private function saveCart() {
    $_SESSION[$this->session_key] = $this->cart;
  }

  /**
   * Returns an array of item_type => plugin for each item type for all active sources.
   *
   * This is used to store items in the cart and
   * during checkout. Each source's 'Request Translation'
   * submit handler has the plugin hard coded when calling
   * $job->addItem, i.e. $job->addItem('locale', $type, $item);
   *
   * During checkout, we must know the the plugin name and item type.
   *
   * @return array $source_map
   */
  private function getSourceMap() {

    $source_map = array();

    foreach (tmgmt_source_plugin_info() as $plugin => $info) {
      foreach ($info['item types'] as $item_type => $type) {
        $source_map[$item_type] = $plugin;
      }
    }
    $source_map['node'] = 'node';

    return $source_map;

  }

}
