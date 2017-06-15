<?php
/**
 * @file
 * User can history of lab migration proposals.
 */
function user_lab_migration_proposals_progress($approval_status = 0) { 
$li[] = ''; 
$li[2] = '';
if ($approval_status >= 0 && $approval_status < 4) {
  $li[1] = 'active';
  }
  if ($approval_status >= 1 && $approval_status < 4) {
    $li[2] = 'active';
  }
  if ($approval_status == 2) {
    $li[2] = 'active';
    drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/rejected.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
  }
  if ($approval_status == 3) {
    $li[3] = 'active';
  }
  else $li[3] = '';

  return '
			<div id="progressbar_container">
				<ul id="progressbar">
					<li class="active"><p class="hidden">Submit Proposal</p></li>
					<li class="' . $li[1] . '"><p class="hidden">Proposal waiting for approval</p></li>
					<li class="' . $li[2] . '"><p class="hidden"><span id="approval">approved waiting for Code Submission</span><span id="rejected">Proposal Rejected</span></p></li>
					<li class="' . $li[3] . '"><p class="hidden">Solution recieved and approved. Lab is completed</p></li>
				</ul>
			</div>';
}

function user_lab_migration_proposals() {
  drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/custom.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
  drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/custom_lab_migration.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
  $contents = '';
  //Get all proposal of current user(history) in table form.
  $proposal_rows = array();
  $query = db_select('lab_migration_proposal');
  $query->fields('lab_migration_proposal');
  $query->orderBy('id', 'DESC');
  $query->condition('uid', $GLOBALS['user']->uid);
  $proposal_q = $query->execute();
  $p = $proposal_q->fetchObject();
  if (!$p) {
    $contents .= t('You have not submitted any proposal yet. ') . l('Click here', 'lab_migration/proposal') . t(' to submit a proposal.');
    return $contents;
  }
  $contents .= '<h4>Current Proposal</h4> Proposal ID: ' . $p->id;
  $contents .= user_lab_migration_proposals_progress($p->approval_status);
  $contents .= '<h4>Previous Proposals</h4>';

  while ($proposal_data = $proposal_q->fetchObject()) {
    $approval_status = '';
    switch ($proposal_data->approval_status) {
      case 0:
        $approval_status = 'Pending';
        break;

      case 1:
        $approval_status = 'Approved';
        break;

      case 2:
        $approval_status = 'Dis-approved';
        break;

      case 3:
        $approval_status = 'Solved';
        break;

      default:
        $approval_status = 'Unknown';
        break;
    }
    $proposal_rows[] = array(
      $proposal_data->id,
      date('d-m-Y', $proposal_data->creation_date),
      $proposal_data->lab_title,
      $proposal_data->department,
      $approval_status,
    );
  }

  $proposal_header = array(
    'Proposal ID',
    'Date of Submission',
    'Title of the Lab',
    'Department',
    'Status',
  );
  $contents .= theme('table', array(
      'header' => $proposal_header,
      'rows' => $proposal_rows,
    ));
  return $contents;
}

