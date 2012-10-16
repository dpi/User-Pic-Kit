<?php

/**
 * @file
 * API documentation for User Pic Kit.
 */

/**
 * Return array()
 */
function hook_userpickit_info() {
  $picture_types = array();

  $picture_types['picture_type_1'] = array( // machine name
    'title' => t('My Picture Type'),
    'description' => t('A description of my picture type.'),
    // Description show in administration interface.
    'default callback' => 'my_picture_type_default',
    // (optional) Path to default image if hook_userpickit_picture() does
    // not return a picture.
    // callback arguments: ($picture_type, $account = NULL)
  );

  return $picture_types;
}

/**
 * Get the picture for a user.
 *
 * If a picture is not available. Return FALSE and the 'default' picture
 * implemented by hook_userpickit_info *may* be used, depending on user
 * preferences.
 *
 * If the file is hosted remotely, and is not being cached locally, then this
 * hook must obey width and height passed into $options. (@todo WIP)
 * 
 * If the width or height cannot be tranformed remotely, then it should be 
 * cached locally so Drupal can do the modification itself.
 *
 * @param object $picture_type
 *   Picture type.
 * @param object $account
 *   User account. An anonymous account may be passed. Do not expect a fully
 *   loaded account object.
 * @param array $options
 *   KEEP REMOTE or CACHE LOCALLY?
 *
 * @return array|NULL
 *   If null is returned, the 'default callback' for this picture type will be
 *   called. If undefined, then the system default image will be rendered.
 */
function hook_userpickit_picture($picture_type, $account) {
  if (!empty($account->uid)) {
    // Registered user.
  }
  else {
    // Anonymous user.
  }

  // return 'fid' OR 'uri'. If one is not defined, default callback will be
  // called.
  return array(
    'fid',
    // File ID. Fill this value if the file is already stored locally.
    'uri',
    // Full uri to image, http://, public:// etc.
    'cache lifetime',
    // Seconds to wait before automatically running this hook again.
    // Return USERPICKIT_CACHE_EXPIRE_NEVER to never expire.
    // The cache may be forced to expire.
    'message',
    // Personalized message for the user displayed on user picture type ui.
  );

  return FALSE;
}


/**
 * Implement default image callback.
 *
 * @param object $picture_type
 *   Picture type.
 * @param object $account
 *   User object.
 * 
 * @see hook_userpickit_info().
 */
function my_picture_type_default($picture_type, $account = NULL) {
  return 'http://www.example.com/test.jpg';
}