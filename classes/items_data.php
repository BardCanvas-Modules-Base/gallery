<?php
namespace hng2_modules\gallery;

use hng2_base\repository\media_record;
use hng2_tools\record_browser;

class items_data
{
    /**
     * @var record_browser
     */
    public $browser = null;
    
    /**
     * @var int
     */
    public $count = 0;
    
    /**
     * @var array
     */
    public $pagination = array();
    
    /**
     * @var media_record[]
     */
    public $items = array();
}
