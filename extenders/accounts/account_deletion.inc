<?php
/**
 * Accounts extender - deletion
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Globals:
 * @var module $current_module accounts
 * @var module $this_module    self
 */

use hng2_base\module;

global $database, $account;

$id_account = $config->globals["deleting_id_account"];
if( empty($id_account) ) return;

if( empty($config->globals["deletions_log"]) ) $config->globals["deletions_log"] = array();
$config->globals["deletions_log"][] = "<b>Starting Gallery extender for account deletion</b>";

#
# Delete all views/tags/mentions/categories of media items for this user
#

$count = $database->exec("
    delete from media_views where id_media in (
      select id_media from media where id_author = '$id_account'
    )
");
if( $count > 0 ) $config->globals["deletions_log"][] = "$count records from <b>media views</b> table deleted.";

$count = $database->exec("
    delete from media_tags where id_media in (
      select id_media from media where id_author = '$id_account'
    )
");
if( $count > 0 ) $config->globals["deletions_log"][] = "$count records from <b>media tags</b> table deleted.";

$count = $database->exec("delete from media_mentions where id_account = '$id_account'");
if( $count > 0 ) $config->globals["deletions_log"][] = "$count mentions of the user from <b>media mentions</b> table deleted.";

$count = $database->exec("
    delete from media_mentions where id_media in (
      select id_media from media where id_author = '$id_account'
    )
");
if( $count > 0 ) $config->globals["deletions_log"][] = "$count records owned by the user in <b>media mentions</b> table deleted.";

$count = $database->exec("
    delete from media_categories where id_media in (
      select id_media from media where id_author = '$id_account'
    )
");
if( $count > 0 ) $config->globals["deletions_log"][] = "$count records from <b>media categories</b> table deleted.";

#
# Grab all media items and delete files and thumbnails
#

$res = $database->query("select id_media, path, thumbnail from media where id_author = '$id_account'");
if( $database->num_rows($res) > 0 )
{
    while($row = $database->fetch_object($res))
    {
        $files = array(
            "{$config->datafiles_location}/uploaded_media/{$row->path}",
            "{$config->datafiles_location}/uploaded_media/{$row->path}.hidden",
            "{$config->datafiles_location}/uploaded_media/{$row->thumbnail}",
            "{$config->datafiles_location}/uploaded_media/{$row->thumbnail}.hidden",
        );
        
        foreach($files as $file)
            if( @unlink($file) )
                $config->globals["deletions_log"][] = "File '$file' deleted";
    }
}

#
# Delete all media records from the table
#

$count = $database->exec("delete from media where id_author = '$id_account'");
if( $count > 0 ) $config->globals["deletions_log"][] = "$count records from <b>media</b> table deleted.";

$config->globals["deletions_log"][] = "<b>Finished Gallery extender for account deletion</b>";
if( $config->globals["notify_deletion_progress"] )
{
    send_notification(
        $account->id_account, "information", "Finished Gallery extender for account deletion"
    );
    sleep(1);
}
