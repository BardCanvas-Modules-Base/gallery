<?php
/**
 * Module installation file
 *
 * @package    BardCanvas
 * @subpackage gallery
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * Trailing vars:
 * @var string $module_install_action install, uninstall, enable, disable
 * @var array  $messages
 * @var array  $errors
 * @var module $this_module
 * @var module $current_module
 */

use hng2_base\module;

$tables = array(
    "media" => "
        create table if not exists media (
            
            id_media          varchar(32) not null default '',
            id_author         varchar(32) not null default '',
            
            # Path is relative to the /media_server directory, E.G.:
            # Relative> /year/month/username_mediatitle_randomseed.png
            # Absolute> /mediaserver/year/month/username_mediatitle_randomseed.png
            # /media_server is located in /data/uploaded_media
            path              varchar(255) not null default '',
            type              varchar(64) not null default '',
            mime_type         varchar(64) not null default '',
            dimensions        varchar(64) not null default '',
            size              int unsigned not null default 0,
            title             varchar(255) not null default '',
            thumbnail         varchar(255) not null default '',
            
            description       text,
            main_category     varchar(32) not null default '',
            
            visibility        enum('public', 'private', 'users', 'friends', 'level_based') not null default 'public',
            status            enum('draft', 'published', 'reviewing', 'hidden', 'trashed') not null default 'draft',
            password          varchar(32) not null default '',
            allow_comments    tinyint unsigned not null default 1,
            
            creation_date     datetime default null,
            creation_ip       varchar(15) not null default '',
            creation_host     varchar(255) not null default '',
            creation_location varchar(255) not null default '',
            
            publishing_date   datetime default null,
            views             int unsigned not null default 0,
            comments_count    int unsigned not null default 0,
            tags              varchar(255) not null default '',
            
            last_update       datetime default null,
            last_viewed       datetime default null,
            last_commented    datetime default null,
            
            primary key        ( id_media ),
            index   by_path    ( path(5) ),
            index   by_author  ( id_author )
            
        ) engine=InnoDB default charset=utf8mb4 collate='utf8mb4_unicode_ci'
    ",
    "media_tags" => "
        create table if not exists media_tags (
            
            id_media       varchar(32) not null default '',
            tag            varchar(100) not null default '',
            date_attached  datetime,
            order_attached double unsigned not null default 0,
            
            primary key ( id_media, tag )
            
        ) engine=InnoDB default charset=utf8mb4 collate='utf8mb4_unicode_ci'
    ",
    "media_categories" => "
        create table if not exists media_categories (
            
            id_media       varchar(32) not null default '',
            id_category    varchar(32) not null default '',
            date_attached  datetime,
            order_attached double unsigned not null default 0,
            
            primary key ( id_media, id_category )
            
        ) engine=InnoDB default charset=utf8mb4 collate='utf8mb4_unicode_ci'
    ",
    "media_mentions" => "
        create table if not exists media_mentions (
            
            id_media       varchar(32) not null default '',
            id_account     varchar(32) not null default '',
            date_attached  datetime,
            order_attached double unsigned not null default 0,
            
            primary key ( id_media, id_account )
            
        ) engine=InnoDB default charset=utf8mb4 collate='utf8mb4_unicode_ci'
    ",
);

if( $module_install_action == "install" )
{
    foreach($tables as $table_name => $query)
    {
        try
        {
            $database->exec($query);
            $messages[] = replace_escaped_vars($language->install_messages->table_created_ok, '{$table_name}', $table_name);
        }
        catch( \Exception $e )
        {
            $errors[] = replace_escaped_vars(
                $language->install_messages->table_created_ko,
                array( '{$table_name}', '{$error}' ),
                array(   $table_name,     $e->getMessage()  )
            );
            
            return;
        }
    }
    
    return;
}

if( $module_install_action == "uninstall" )
{
    foreach($tables as $table_name => $query)
    {
        try
        {
            $database->exec("DROP TABLE if exists {$table_name}");
            $messages[] = replace_escaped_vars($language->install_messages->table_deleted_ok, '{$table_name}', $table_name);
        }
        catch( \Exception $e )
        {
            $errors[] = replace_escaped_vars(
                $language->install_messages->table_deleted_ko,
                array( '{$table_name}', '{$error}' ),
                array(   $table_name,     $e->getMessage() )
            );
        }
    }
    
    return;
}
