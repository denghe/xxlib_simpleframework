#pragma warning disable 0169, 0414
using TemplateLibrary;

namespace Tables
{
    /*
CREATE TABLE `manager` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(50) NOT NULL DEFAULT '0',
  `password` varchar(50) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='管理人员表'
    */
    [Desc(@"管理人员表")]
    class manager
    {
        [Desc(@"")]
        [Column(true)] int id;
        [Desc(@"")]
        [Column] string username;
        [Desc(@"")]
        [Column] string password;
    }
    /*
CREATE TABLE `manager_role` (
  `manager_id` int(11) NOT NULL,
  `role_id` int(11) NOT NULL,
  PRIMARY KEY (`manager_id`,`role_id`),
  KEY `FK_manager_role_role` (`role_id`),
  CONSTRAINT `FK_manager_role_manager` FOREIGN KEY (`manager_id`) REFERENCES `manager` (`id`),
  CONSTRAINT `FK_manager_role_role` FOREIGN KEY (`role_id`) REFERENCES `role` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='管理人员_身份_绑定表'
    */
    [Desc(@"管理人员_身份_绑定表")]
    class manager_role
    {
        [Desc(@"")]
        [Column] int manager_id;
        [Desc(@"")]
        [Column] int role_id;
    }
    /*
CREATE TABLE `permission` (
  `id` int(11) NOT NULL,
  `group` varchar(50) NOT NULL,
  `name` varchar(50) NOT NULL,
  `desc` varchar(250) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='权限表'
    */
    [Desc(@"权限表")]
    class permission
    {
        [Desc(@"")]
        [Column] int id;
        [Desc(@"")]
        [Column] string group;
        [Desc(@"")]
        [Column] string name;
        [Desc(@"")]
        [Column] string desc;
    }
    /*
CREATE TABLE `role` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL DEFAULT '0',
  `desc` varchar(250) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='管理人员身份表'
    */
    [Desc(@"管理人员身份表")]
    class role
    {
        [Desc(@"")]
        [Column(true)] int id;
        [Desc(@"")]
        [Column] string name;
        [Desc(@"")]
        [Column] string desc;
    }
    /*
CREATE TABLE `role_permission` (
  `role_id` int(11) NOT NULL,
  `permission_id` int(11) NOT NULL,
  PRIMARY KEY (`role_id`,`permission_id`),
  KEY `FK_role_permission_permission` (`permission_id`),
  CONSTRAINT `FK_role_permission_permission` FOREIGN KEY (`permission_id`) REFERENCES `permission` (`id`),
  CONSTRAINT `FK_role_permission_role` FOREIGN KEY (`role_id`) REFERENCES `role` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='身份_权限_绑定表'
    */
    [Desc(@"身份_权限_绑定表")]
    class role_permission
    {
        [Desc(@"")]
        [Column] int role_id;
        [Desc(@"")]
        [Column] int permission_id;
    }
}
