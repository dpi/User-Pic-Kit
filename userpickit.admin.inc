<?php

/**
 * @file
 * Pages for User Pic Kit.
 */

/**
 * Implements hook_form_BASE_FORM_ID_alter().
 *
 * Hook into user_profile_form().
 *
 * @see hook_form_BASE_FORM_ID_alter()
 */
function userpickit_form_user_profile_form_alter(&$form, &$form_state, $form_id) {
  drupal_add_css(drupal_get_path('module', 'userpickit') . '/css/userpickit.css');

  $picture_types = userpickit_types_all();
  $form['#submit'][] = 'userpickit_form_user_submit';
  $account = $form['#user'];
  $options = array();

  foreach ($picture_types as $picture_type => $info) {
    if (user_access('userpickit use ' . $picture_type . ' type', $account)) {
      $option = $info['title'];

      // Image
      $style = _userpickit_picture_type_ui_style();
      $path = userpickit_user_picture_path($account, $picture_type);
      $option .= '<br />' . userpickit_render_picture($account, $path, $style);

      $picture_type_info = userpickit_user_picture_type_info($picture_type, $account);
      if (array_key_exists('message', $picture_type_info)) {
        $option .= '<br />' . $picture_type_info['message'];
      }

      $options[$picture_type] = $option;
    }
  }

  $form['picture']['picture_type'] = array(
    '#title' => t('Picture'),
    '#description' => t('Choose a picture to associate with your account.'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => userpickit_user_picture_type_get($account),
    '#weight' => -10,
    '#access' => !empty($options),
    '#theme' => 'userpickit_selector',
  );
  $form['picture']['#title'] = t('Picture');
  $form['picture']['#type'] = 'fieldset';

  $form['picture']['picture_current']['#access'] = FALSE;
  $form['picture']['picture_delete']['#access'] = FALSE;
}

/**
 * Form submission callback.
 * 
 * @see userpickit_form_user_profile_form_alter()
 */
function userpickit_form_user_submit($form, $form_state) {
  $account = $form['#user'];
  userpickit_user_picture_reset($account->uid);
}

/**
 * @see hook_user_presave().
 *
 * Saves data from user_profile_form.
 *
 * @see userpickit_form_user_profile_form_alter().
 */
function _userpickit_user_presave_admin(&$edit, $account, $category) {
  if (isset($edit['picture_type'])) {
    $edit['data']['userpickit_type'] = $edit['picture_type'];
  }
}

/**
 * Global settings form for User Pic Kit.
 */
function userpickit_settings_form($form, $form_state) {
  /**
   * Global settings:
   *
   *   Default picture type.
   * 
   *   If a picture cannot be retrieved, or if a user has chosen a uncachable
   *   remote picture then the following behavior should be used:
   *
   *    * Set user picture to nothing
   *    * Leave user picture as last successful picture 
   *   
   *   @see userpickit_user_picture_reset()

       * Cache time for inactive pictures
   * 
   * Per picture type settings:
   * 
   * Caching - Allow admin to choose whether an picture type should be downloaded
   * and cached locally.
   *    KEEP_REMOTE : link directly.
   *    CACHE_LOCAL
   *   
   */
  $options = array();
  $picture_types = userpickit_types_all();
  foreach ($picture_types as $picture_type => $info) {
    $options[$picture_type] = $info['title'];
  }

  $form['picture_type_default'] = array(
    '#title' => t('Default picture type'),
    '#description' => t('Picture type to use when a user is anonymous, or has not set a preference.'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => variable_get('userpickit_default', '')
  );

  if (module_exists('image')) {
    $form['picture_type_ui_style'] = array(
      '#type' => 'select',
      '#title' => t('Picture list display style'),
      '#options' => image_style_options(TRUE),
      '#default_value' => _userpickit_picture_type_ui_style(),
      '#description' => t('The style of image to use on the picture configuration form. Styles may be configured in the <a href="!url">Image styles</a> administration area.', array('!url' => url('admin/config/media/image-styles'))),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  return $form;
}

/**
 * Form submission callback
 *
 * @see userpickit_settings_form()
 */
function userpickit_settings_form_submit($form, &$form_state) {
  variable_set('userpickit_default', $form_state['values']['picture_type_default']);
  variable_set('userpickit_picture_type_ui_style', $form_state['values']['picture_type_ui_style']);
}

/**
 * Get current image style for picture configuration form.
 */
function _userpickit_picture_type_ui_style() {
  return variable_get(
    'userpickit_picture_type_ui_style',
    variable_get('user_picture_style', '')
  );
}