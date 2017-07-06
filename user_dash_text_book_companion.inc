<?php

/**
 * @file
 * User can history of Book proposals.
 */
function user_text_book_companion_proposals() {
  if (user_access('access user_dash')) {
    //Create a empty string which will store html contents.
    $contents = '';
    //Create a empty array for storing the table rows.
    $book_rows = array();
    //Get data from {textbook_companion_proposal} and {textbook_companion_preference} tables with help of inner join.
    $result = db_query('SELECT textbook_companion_proposal.id,
		textbook_companion_preference.book_status,
		textbook_companion_proposal.new_proposal_status,
		textbook_companion_preference.book,
		textbook_companion_preference.edition,
		textbook_companion_preference.author,
		textbook_companion_preference.year
		FROM {textbook_companion_proposal} INNER JOIN {textbook_companion_preference} ON textbook_companion_preference.proposal_id=textbook_companion_proposal.id
		where textbook_companion_proposal.uid= :uid;',
      array(':uid' => $GLOBALS['user']->uid)
    );

    //Fetch the first object(proposal) for displaying progress bar.
    $p = $result->fetchObject();
    if (!$p) {
      //If no proposal Found
      $contents .= t('You have not submitted any books proposal.') . l('Click here', 'book_proposal') . t(' to submit.');
      return $contents;
    }
    //Display current proposal details.
    $contents .= '<div id="selected_proposal"><h4>Current Proposal:</h4>Proposal ID: ' . $p->id;
    $contents .= '<br />Book: '. $p->book .' [ ' . $p->edition . ' ]';
    $contents .= '<br /><em>by ' . $p->author;
    //Create a empty progressBar and append to html contents.
    $contents .= user_tbc_progress();
    //Call the javascript function and fill the progress bar.
    $contents .= '<h4>Previous Proposals</h4><script>selectProposal_TBC(' . $p->new_proposal_status . ',' . $p->book_status . ');</script>';
    //Iterate over the remaining rows.
    while ($book_data = $result->fetchObject()) {
      //Get book Status.
      $book_status = getBookStatus($book_data->book_status);
      //Fill them rows with data.
      $book_rows[] = array(
        $book_data->id,
        $book_data->book . '<br> <em>by ' . $book_data->author . '</em>',
        $book_data->edition,
        $book_data->year,
        $book_data->new_proposal_status,
        $book_status,
      );
    }
    if (!$book_rows) {
      //If no previous proposal are found.
      return $contents .= t('There are no previous proposal.');
    }
    //Create header for html table.
    $book_header = array(
      'Proposal ID',
      'Book',
      'Edition',
      'Year',
      'Proposal Status',
      'Book Status',
    );
    //Create html table and append it to html contents.
    $contents .= theme('table', array(
        'header' => $book_header,
        'rows' => $book_rows,
      ));
    //return contents to render it on page.
    return $contents;
  }
}

function user_tbc_progress() {
  // Return a string with html content of progress bar based on status
  drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/custom.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
  drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/custom_tbc.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
  drupal_add_css(drupal_get_path('module', 'user_dash') . '/css/rejected.css', array('group' => CSS_DEFAULT, 'every_page' => FALSE));
  drupal_add_js(drupal_get_path('module', 'user_dash') . '/js/progressBar.js', 'file');
  return '
			<div id="progressbar_container">
				<ul id="progressbar">
					<li id="node1"><p class="hidden">Submit Book Proposal</p></li>
					<li id="node2"><p class="hidden">Proposal Recieved, Waiting for approval</p></li>
					<li id="node3"><p class="hidden"><span id="approved">Proposal Approved, Start '.l('submitting codes','/textbook_companion/code').'</span><span id="rejected">Proposal Rejected</span></p></li>
					<li id="node4"><p class="hidden">Code received. Under review</p></li>
					<li id="node5"><p class="hidden">Code reviewed and Completed Book</p></li>
					<li id="node6"><p class="hidden">Book Completed</p></li>
				</ul>
			</div>
		</div>';
}

function getProposalStatus($proposal_status = 0) {
  //Returns status of propsoal.
  switch ($proposal_status) {
    case 0:
      return 'Under Review';

    case 1:
      return 'Accepted';

    case 2:
      return 'Rejected';

    default:
      return 'Unknown';
  }
}

function getBookStatus($book_status = 0) {
  //Returns status of book.
  switch ($book_status) {
    case 0:
      return 'In Progress';

    case 1:
      return 'Under Review';

    case 2:
      return 'Approved';

    case 3:
      return 'Completed';

    case 4:
      return 'Disabled';

    default:
      return 'Unknown';
  }
}

