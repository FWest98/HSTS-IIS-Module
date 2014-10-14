using Microsoft.Web.Management.Server;
using System;
using System.Collections.Generic;
using System.Text;

namespace HSTS_IIS_Module.Manager
{
    class ConfigModuleProvider : ModuleProvider
    {
        public override Type ServiceType
        {
            get { return null; }
        }

        public override bool SupportsScope(ManagementScope scope)
        {
            return true;
        }

        public override ModuleDefinition GetModuleDefinition(IManagementContext context)
        {
            return new ModuleDefinition(Name, typeof(ConfigModule).AssemblyQualifiedName);
        }
    }
}
