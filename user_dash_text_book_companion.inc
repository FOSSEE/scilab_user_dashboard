<?php
/**
 * @file
 * User can history of Book proposals.
 */
function user_tbc_progress($proposal_status = 0, $book_status = 0) {
// Return a string with html content of progress bar based on status

	$li[] = '';
	$li[2] = '';
	$li[3] = '';
	$li[4] = '';
	if ($proposal_status >= 0 && $proposal_status < 2) {
	//Proposal Recieved
	$li[0] = 'active';
	}
	if ($proposal_status = 1){
	//Proposal approved
		if($book_status > 0 && $book_status < 5){
		//Code Recieved
			$li[2] = 'active';
		}
		if($book_status > 1 && $book_status < 5){
		//Code reviewed
			$li[3] = 'active';
		}
		if($book_status > 2 && $book_status < 5){
		//Book Completed
			$li[4] = 'active';
		}
	}
	if ($proposal_status = 2){
	//Proposal rejected
		drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/rejected.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
	}
	return '
			<div id="progressbar_container">
				<ul id="progressbar">
					<li class="active"><p class="hidden">Submit Book Proposal</p></li>
					<li class="' . $li[0] . '"><p class="hidden">Proposal Recieved, Waiting for approval</p></li>
					<li class="' . $li[1] . '"><p class="hidden"><span id="approved">Proposal Approved, Start submitting codes</span><span id="rejected">Proposal Rejected</span></p></li>
					<li class="' . $li[2] . '"><p class="hidden">Code received. Under review</p></li>
					<li class="' . $li[3] . '"><p class="hidden">Code reviewed and Completed Book</p></li>
					<li class="' . $li[4] . '"><p class="hidden">Book Completed</p></li>
				</ul>
			</div>';
}

function user_text_book_companion_proposals() {
  drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/custom.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
  drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/custom_tbc.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
  $contents = '';

  /* Get proposal db where user id is of current user.*/
  $book_rows = array();
  $result = db_query('SELECT textbook_companion_proposal.id,
					textbook_companion_preference.book_status,
					textbook_companion_proposal.new_proposal_status,
					textbook_companion_preference.book,
					textbook_companion_preference.edition,
					textbook_companion_preference.author,
					textbook_companion_preference.year
					FROM {textbook_companion_proposal} INNER JOIN {textbook_companion_preference} ON textbook_companion_preference.proposal_id=textbook_companion_proposal.id
					where textbook_companion_proposal.uid= :uid;',
					array(':uid' => $GLOBALS['user']->uid));
					
	//Fetch the first object(proposal) for displaying progress bar.
  $p = $result->fetchObject();
  if (!$p) {
	//If no proposal Found
    $contents .= t('You have not submitted any books proposal.') . l('Click here', 'book_proposal') . t(' to submit.');
    return $contents;
  }
  $contents .= '<h4>Current Proposal</h4> Book: ' . $p->book . '<br /> Proposal ID: ' . $p->id;
//   dpm(get_defined_vars());
  $contents .= user_tbc_progress($p->new_proposal_status, $p->book_status);

  $contents .= '<h4>Previous Proposals</h4>';

  while ($book_data = $result->fetchObject()) {
    /* get preference */


    /*$preference_q = db_query("SELECT * FROM textbook_companion_preference WHERE proposal_id = %d AND approval_status = 1 LIMIT 1", $proposal_data->id);
			$preference_data = db_fetch_object($preference_q);*/


    $book_status = '';
    switch ($book_data->book_status) {
      case 0:
        $book_status = 'In Progress';
        break;

      case 1:
        $book_status = 'Under Review';
        break;

      case 2:
        $book_status = 'Approved';
        break;

      case 3:
        $book_status = 'Completed';
        break;

      case 4:
        $book_status = 'Disabled';
        break;

      default:
        $book_status = 'Unknown';
        break;
    }
    //book_status
    $book_rows[] = array(
      $book_data->id,
      $book_data->book . '<br> <em>by ' . $book_data->author . '</em>',
      $book_data->edition,
      $book_data->year,
      $book_data->new_proposal_status,
      $book_status,
    );
  }
  //$proposal_data = $proposal_q->fetchObject()
  /* check if there are any pending proposals */

  if(!$book_rows){
	//If no previous proposal are found.
    return $contents .= t('There are no previous proposal.');
  }

  //!$proposal_rows
  $book_header = array(
    'Proposal ID',
    'Book',
    'Edition',
    'Year',
    'Proposal Status',
    'Book Status',
  );
  $contents .= theme('table', array(
      'header' => $book_header,
      'rows' => $book_rows,
    ));
  return $contents;
}
