node('linux') {
  dir('work') {
    def workDir = pwd()

    dir('build') {
      deleteDir()
    }

    dir('output') {
      deleteDir()
    }

    dir('install') {
      deleteDir()
    }

    dir('src') {
      checkout([
        $class: 'GitSCM',
        branches: scm.branches,
        doGenerateSubmoduleConfigurations: scm.doGenerateSubmoduleConfigurations,
        extensions: scm.extensions + [[$class: 'CloneOption', noTags: false, reference: '', shallow: true]],
        submoduleCfg: [],
        userRemoteConfigs: scm.userRemoteConfigs
      ])

      def gitDir = pwd()

      withEnv(["WORK_FOLDER=${workDir}", "GIT_FOLDER=${gitDir}"]) {
        sh "chmod +x ./build-qemu.sh && ./build-qemu.sh --helper-script ./build-helper.sh --win32 --deb32 --deb64"
      }
    }
  }

  step([$class: 'ArtifactArchiver', artifacts: 'work/install/win32/qemu/**/*', fingerprint: false])
  step([$class: 'ArtifactArchiver', artifacts: 'work/install/debian32/qemu/**/*', fingerprint: false])
  step([$class: 'ArtifactArchiver', artifacts: 'work/install/debian64/qemu/**/*', fingerprint: false])
}
