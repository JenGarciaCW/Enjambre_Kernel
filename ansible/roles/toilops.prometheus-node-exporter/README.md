Prometheus Node Exporter
=========

[![Build Status](https://travis-ci.org/toilops/prometheus-node-exporter.svg?branch=master)](https://travis-ci.org/toilops/prometheus-node-exporter)   [![Ansible-Galaxy](https://img.shields.io/badge/Ansible--Galaxy-toilops.prometheus--node--exporter-blue.svg)](https://galaxy.ansible.com/toilops/prometheus-node-exporter)

Ansible Role to configure and install Prometheus Node Exporter. Role is tagged based on the Prometheus release version.

Requirements
------------

Running Linux machine ready for Prometheus Node Exporter to be installed.

Role Variables
--------------

Installation of Node Exporter is straight forward and extremely light weight. The following variables can be changed to meet your requirements.

User to run Node Exporter under, the role will create the below user by default.

```yaml
pne_user_id: prometheus
pne_user_gid: prometheus
```

Disable user creation by setting the following to false. You must set `pne_user_id` and `pne_user_gid` to the required runtime user.
```yaml
pne_create_user: false
```

Installation Directory

```yaml
pne_install_dir: /opt/prometheus/bin
pne_systemd_dir: /etc/systemd/system
```

Variables to define which Node Exporter binary to download.

```yaml
pne_system: "{{ ansible_system | lower }}"
pne_app_name: node_exporter
pne_version: 0.15.2
pne_base_url: https://github.com
pne_context_root: prometheus/node_exporter/releases/download
pne_file_name: "{{ pne_app_name }}-{{ pne_version }}.{{ pne_system }}-{{ pne_arch_type }}.{{ pne_file_extension }}"
pne_download_uri: "{{ pne_base_url }}/{{ pne_context_root }}/{{ pne_tag_version }}/{{ pne_file_name }}"
```

Example Playbook
----------------

The following is an example of how to run this role on your remote system:

    - hosts: servers
      roles:
         - { role: toilops.prometheus-node-exporter, pne_version: 0.15.1 }

License
-------

Apache

Author Information
------------------

Find me on Github [@BondAnthony](https://github.com/BondAnthony)
