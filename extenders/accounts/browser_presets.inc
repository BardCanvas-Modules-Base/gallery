<?php
/**
 * Accounts extender - browser presets
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Globals:
 * @var module $current_module accounts
 * @var module $this_module    self
 * @var mixed  $order
 */

use hng2_base\module;

global $order;

list($sqlorder, $additional_select_columns) = $config->globals["accounts:nav/presettings_data"];

switch( $order )
{
    case "media_count_asc":  $sqlorder = "_media_count asc";  break;
    case "media_count_desc": $sqlorder = "_media_count desc"; break;
}

$additional_select_columns .= "
    , (select count(id_media) from media where media.id_author = account.id_account ) as _media_count
";

$config->globals["accounts:nav/presettings_data"] = array( $sqlorder, $additional_select_columns);
