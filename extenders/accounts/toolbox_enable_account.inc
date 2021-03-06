<?php
/**
 * Accounts extender - toolbox operations
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Globals:
 * @var module  $current_module accounts
 * @var module  $this_module    self
 * @var account $user_account
 */

use hng2_base\account;
use hng2_base\module;
use hng2_media\media_repository;

global $user_account;

$repository = new media_repository();
$count      = $repository->unhide_all_published_by_auhtor($user_account->id_account);
if( empty($count) ) return;

broadcast_to_moderators("information", replace_escaped_vars(
    $this_module->language->messages->items_unhidden,
    array('{$count}', '{$author}'),
    array($count, convert_emojis($user_account->display_name))
));
