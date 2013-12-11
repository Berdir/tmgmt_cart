<?php

/**
 * Implements hook_block_info()
 *
 * @return array $blocks
 */
function tmgmt_cart_block_info() {

  $blocks = array();

  $blocks['tmgmt_cart_contents'] = array(
    'info' => t('TMGMT Job Items Cart'),
    'cache' => DRUPAL_NO_CACHE,
    'visibility' => BLOCK_VISIBILITY_LISTED,
    'pages' => 'admin/tmgmt/sources*',
  );

  return $blocks;

}

/**
 * Implements hook_block_configure()
 *
 * @param string $delta
 */
function tmgmt_cart_block_configure($delta = '') {

  $form = array();

  switch ($delta) {

    case 'tmgmt_cart_contents':

      $form['visibility']['empty'] = array(
        '#type' => 'fieldset',
        '#title' => t('Empty Cart'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#group' => 'visibility',
        '#weight' => 100,
      );

      $form['visibility']['empty']['tmgmt_cart_hide_empty_cart'] = array(
        '#type' => 'checkbox',
        '#title' => t('Hide the cart if empty'),
        '#default_value' => variable_get('tmgmt_cart_hide_empty_cart', 0),
      );

      break;
  }

  return $form;

}

/**
 * Implements hook_block_save()
 *
 * @param string $delta
 * @param array $edit
 */
function tmgmt_cart_block_save($delta = '', $edit = array()) {

  if ($delta == 'tmgmt_cart_contents') {
    variable_set('tmgmt_cart_hide_empty_cart', $edit['tmgmt_cart_hide_empty_cart']);
  }

}

/**
 * Implements hook_block_view()
 *
 * @param string $delta
 * @return array $block
 */
function tmgmt_cart_block_view($delta = '') {

  switch ($delta) {

    case 'tmgmt_cart_contents':

      $cart = TMGMTCart::getInstance();
      $items = $cart->getItems();

      if (count($items) === 0 && variable_get('tmgmt_cart_hide_empty_cart', 0) === 1) {
        return NULL;
      }

      $block['subject'] = t('Job Cart');
      $block['content'] = _tmgmt_cart_contents($items);
      break;

  }

  return $block;

}

/**
 * Callback function to generate markup
 * for the 'shopping' cart of job items
 *
 * @return string $markup
 */
function _tmgmt_cart_contents(array $items) {
  return drupal_get_form('tmgmt_cart_cart_form', $items);
}
