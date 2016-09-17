<?php
/**
 * Accounts extender - browser data columns
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Globals:
 * @var module $current_module accounts
 * @var module $this_module    self
 * @var object $row
 */

use hng2_base\module;

global $row;
?>
<td nowrap align="right">
    <?
    if( empty($row->_media_count) )
        echo "&mdash;";
    else
        echo "<a href='/gallery/?author={$row->id_account}'>" . number_format($row->_media_count) . "</a>";
    ?>
</td>
