<?php
/**
 * Accounts extender - principal data addons
 *
 * @package    BardCanvas
 * @subpackage posts
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Globals:
 * @var module $current_module accounts
 * @var module $this_module    self
 */

use hng2_base\module;

global $row, $principal_data_addons, $modules;

if( empty($row->_last_media_item_data) ) return;

list($id, $type, $title, $date, $status) = explode("\t", $row->_last_media_item_data);

$principal_data_addons[] = replace_escaped_vars(
    $this_module->language->accounts_nav_columns->principal_data,
    array('{$url}', '{$type}', '{$title}', '{$time}', '{$status}'),
    array(
        "{$config->full_root_path}/media/{$id}",
        current($this_module->language->xpath("//media_types/media[@type='$type']/caption/text()")),
        convert_emojis($title),
        $date == "0000-00-00 00:00:00" ? "&mdash;" : get_minimized_date($date),
        $this_module->language->status_labels->{$status},
    )
);
