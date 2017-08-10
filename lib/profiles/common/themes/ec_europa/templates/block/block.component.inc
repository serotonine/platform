<?php

/**
 * @file
 * Contains component file.
 */

/**
 * Implements hook_preprocess_hook().
 */
function ec_europa_preprocess_block(&$variables, $hook) {
  $block = $variables['block'];

  switch ($block->delta) {
    case 'nexteuropa_feedback':
      $variables['classes_array'][] = 'block--full-width';
      break;

    case 'menu-nexteuropa-site-links':
    case 'menu-nexteuropa-social-media':
    case 'menu-nexteuropa-inst-links':
      $variables['title'] = $block->subject;
      $variables['title_attributes_array']['class'][] = 'ecl-h4';
      $variables['title_attributes_array']['class'][] = 'ecl-footer__title';
      $block->subject = '';
      break;

    case 'menu-nexteuropa-service-links':
      $variables['title'] = '';
      $variables['title_attributes_array']['class'][] = 'ecl-h4';
      $variables['title_attributes_array']['class'][] = 'ecl-footer__title';
      $block->subject = '';
      break;

    case 'views_related_links':
      $variables['classes_array'][] = 'link-block';
      $variables['title_attributes_array']['class'][] = 'link-block__title';
      break;

    case 'language_selector_site':
      $variables['lang_code'] = $variables['elements']['code']['#markup'];
      $variables['lang_name'] = $variables['elements']['label']['#markup'];
      // Add class to block.
      $variables['classes_array'][] = 'ecl-lang-select-sites';
      $variables['link'] = url('splash') . '?' . drupal_get_destination()['destination'];
      break;

    case 'inline_navigation':
      $variables['classes_array'][] = 'inpage-nav__wrapper';
      $variables['title_attributes_array']['class'][] = 'inpage-nav__block-title';
      break;
  }

  // Page-level language switcher.
  if (isset($block->bid) && $block->bid === 'language_selector_page-language_selector_page') {
    global $language;

    // selectify.js is the library to convert between ul and select.
    drupal_add_js(drupal_get_path('theme', 'ec_europa') . '/assets/js/selectify.js');
    drupal_add_js(drupal_get_path('theme', 'ec_europa') . '/assets/js/components/lang-switcher.js');

    // Initialize variables.
    $not_available = '';
    $served = '';
    $other = '';

    if (!empty($variables['elements']['not_available']['#markup'])) {
      $not_available = '<li class="lang-select-page__option lang-select-page__unavailable">' . $variables['elements']['not_available']['#markup']->native . '</li>';
    }

    if (!empty($variables['elements']['served']['#markup'])) {
      $served = '<li class="lang-select-page__option is-selected">' . $variables['elements']['served']['#markup']->native . '</li>';
    }

    if (!empty($variables['elements']['other']['#markup'])) {
      foreach ($variables['elements']['other']['#markup'] as $code => $lang) {
        $options = [];
        $options['query'] = drupal_get_query_parameters();
        $options['query']['2nd-language'] = $code;
        $options['attributes']['lang'] = $code;
        $options['attributes']['hreflang'] = $code;
        $options['attributes']['rel'] = 'alternate';
        $options['language'] = $language;

        $other .= '<li class="lang-select-page__option lang-select-page__other">' . l($lang->native, current_path(), $options) . '</li>';
      }
    }

    // Add class to block.
    $variables['classes_array'][] = 'lang-select-page lang-select-page--transparent';

    // Add content to block.
    $content = "<span class='lang-select-page__icon icon icon--generic-lang'></span>";
    $content .= "<ul class='lang-select-page__list'>" . $not_available . $served . $other . '</ul>';

    $variables['content'] = $content;
  }

  // Site-level language switcher.
  if (isset($block->bid) && $block->bid === 'language_selector_site-language_selector_site') {
    // Add the js to make it function.
    drupal_add_js(drupal_get_path('theme', 'ec_europa') . '/assets/js/components/lang-select-site.js');
  }

  // Replace block-title class with block__title in order to keep BEM structure
  // of classes.
  if ($block_title_class = array_search('block-title', $variables['title_attributes_array']['class'])) {
    unset($variables['title_attributes_array']['class'][$block_title_class]);
  }
  $variables['title_attributes_array']['class'][] = 'block__title';

  if (isset($block->bid)) {
    // Check if the block is a exposed form.
    // This is checked by looking at the $block->bid which in case
    // of views exposed filters, always contains 'views--exp-' string.
    if (strpos($block->bid, 'views--exp-') !== FALSE) {
      if (isset($block->context) && $context = context_load($block->context)) {
        // If our block is the first, we set the subject. This way, if we expose
        // a second block for the same view, we will not duplicate the title.
        if (array_search($block->bid, array_keys($context->reactions['block']['blocks'])) === 0) {
          $variables['classes_array'][] = 'filters';
          $variables['title_attributes_array']['class'][] = 'filters__title';
          $block->subject = t('Refine results');

          // Passing block id to Drupal.settings in order to pass it through
          // data attribute in the collapsible panel.
          drupal_add_js(array('ec_europa' => array('exposedBlockId' => $variables['block_html_id'])), 'setting');

          // Adding filters.js file.
          drupal_add_js(drupal_get_path('theme', 'ec_europa') . '/assets/js/components/filters.js');
        }
      }
    }
  }

  if (isset($variables['elements']['#markup'])) {
    // If "#markup" is set, that means it is the case of custom block
    // with "body field" that can contain simple HTML tags without "ecl"
    // classes.
    // Then the namespace CSS class must be added.
    $variables['content']['#attributes']['class'][] = 'ecl-editor';
    $variables['content_attributes_array']['class'][] = 'ecl-editor';
  }

  $variables['attributes_array']['class'] = drupal_array_merge_deep($variables['attributes_array']['class'], $variables['classes_array']);
  $variables['attributes_array']['class'][] = 'ecl-clearfix';
  $variables['attributes_array']['id'] = $variables['block_html_id'];
}