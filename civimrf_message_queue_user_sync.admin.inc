<?php

function civimrf_message_queue_user_sync_admin_form($form, &$form_state) {
	$profiles = cmrf_core_list_profiles();
  $profiles_options = array('' => t(' - Default profile -'));
  foreach($profiles as $profile_name => $profile) {
    $profiles_options[$profile_name] = $profile['label'];
  }
	
	$settings = civimrf_message_queue_user_sync_get_settings();
	
	$form['profile']['#type'] = 'select';
  $form['profile']['#title'] = t('CiviMRF Connection profile');
  $form['profile']['#options'] = $profiles_options;
  $form['profile']['#default_value'] = $settings['profile'];
	
	$form['message_queue_name']['#type'] = 'textfield';
  $form['message_queue_name']['#title'] = t('CiviCRM Message Queue name');
	$form['message_queue_name']['#required'] = true;
  $form['message_queue_name']['#default_value'] = $settings['message_queue_name'];

  $form['api_limit']['#type'] = 'textfield';
  $form['api_limit']['#title'] = t('CiviCRM Api Batch Size');
  $form['api_limit']['#description'] = t('Define the size of the batch. Each time when the synchronize job runs it processes one batch, this setting defines how many users a batch can contain.');
  $form['api_limit']['#required'] = true;
  $form['api_limit']['#default_value'] = $settings['api_limit'];
	
	$form['username_attribute']['#type'] = 'textfield';
  $form['username_attribute']['#title'] = t('Username Attribute');
	$form['username_attribute']['#description'] = t('The attribute is the field in the results of the API call and is stored as the username at the user record.');
	$form['username_attribute']['#required'] = true;
  $form['username_attribute']['#default_value'] = $settings['username_attribute'];
	
	$form['email_attribute']['#type'] = 'textfield';
  $form['email_attribute']['#title'] = t('Email Attribute');
	$form['email_attribute']['#description'] = t('The attribute is the field in the results of the API call and is stored as the email at the user record.');
	$form['email_attribute']['#required'] = true;
  $form['email_attribute']['#default_value'] = $settings['email_attribute'];

  $form['send_account_mail']['#type'] = 'checkbox';
  $form['send_account_mail']['#title'] = t('Send account mail');
  $form['send_account_mail']['#description'] = t('When a new user is created send an account mail');
  $form['send_account_mail']['#default_value'] = $settings['send_account_mail'];
  
  $form['hide_username_field']['#type'] = 'checkbox';
  $form['hide_username_field']['#title'] = t('Hide username field');
  $form['hide_username_field']['#description'] = t('Hides the username field from the account page');
  $form['hide_username_field']['#default_value'] = $settings['hide_username_field'];
  
  $form['hide_email_field']['#type'] = 'checkbox';
  $form['hide_email_field']['#title'] = t('Hide email field');
  $form['hide_email_field']['#description'] = t('Hides the e-mail field from the account page');
  $form['hide_email_field']['#default_value'] = $settings['hide_email_field'];
	
	$roles = array_map('check_plain', user_roles(TRUE));
  // The disabled checkbox subelement for the 'authenticated user' role
  // must be generated separately and added to the checkboxes element,
  // because of a limitation in Form API not supporting a single disabled
  // checkbox within a set of checkboxes.
  // @todo This should be solved more elegantly. See issue #119038.
  $checkbox_authenticated = array(
    '#type' => 'checkbox',
    '#title' => $roles[DRUPAL_AUTHENTICATED_RID],
    '#default_value' => TRUE,
    '#disabled' => TRUE,
  );
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' =>  $settings['roles'],
    '#options' => $roles,
    DRUPAL_AUTHENTICATED_RID => $checkbox_authenticated,
  );
	
	$form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }

  $form['#submit'][] = 'civimrf_message_queue_user_sync_admin_form_submit';
  // By default, render the form using theme_system_settings_form().
  $form['#theme'] = 'system_settings_form';
	
	return $form;
}

/**
 * Submit handler for adding a new image style.
 */
function civimrf_message_queue_user_sync_admin_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $profile = $form_state['values']['profile'];
   
  if ($profile) {
    variable_set('civimrf_message_queue_user_sync_profile', $profile);
  } else {
  	variable_del('civimrf_message_queue_user_sync_profile');
	}

	variable_set('civimrf_message_queue_user_name', $form_state['values']['message_queue_name']);
  variable_set('civimrf_message_queue_user_sync_api_limit', $form_state['values']['api_limit']);
	variable_set('civimrf_message_queue_user_sync_username_attribute', $form_state['values']['username_attribute']);
	variable_set('civimrf_message_queue_user_sync_email_attribute', $form_state['values']['email_attribute']);
  variable_set('civimrf_message_queue_user_sync_hide_username_field', $form_state['values']['hide_username_field']);
  variable_set('civimrf_message_queue_user_sync_send_account_mail', $form_state['values']['send_account_mail']);
  variable_set('civimrf_message_queue_user_sync_hide_email_field', $form_state['values']['hide_email_field']);
	variable_set('civimrf_message_queue_user_sync_roles', $form_state['values']['roles']);

	drupal_set_message(t('Settings have been saved'));
  $form_state['redirect'] = 'admin/config/civimrf/civimrf_message_queue_user_sync';
}
