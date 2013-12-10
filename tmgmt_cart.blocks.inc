<?php

/**
 * Implements hook_block_info()
 *
 * @return array $blocks
 */
function tmgmt_cart_block_info() {

  $blocks = array();

  $blocks['tmgmt_cart_contents'] = array(
    'info'  => t('TMGMT Job Items Cart'),
    'cache' => DRUPAL_NO_CACHE,
  );

  return $blocks;

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

      $markup = _tmgmt_cart_contents();

      if (!$markup) {
        return NULL;
      }

      $block['subject'] = t('Job Items');
      $block['content'] = $markup;
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
function _tmgmt_cart_contents() {

  $markup = drupal_get_form('tmgmt_cart_cart_form');
  return $markup;

}
